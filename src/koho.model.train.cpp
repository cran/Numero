/* Created by Ville-Petteri Makinen
   email: vpmakine@gmail.com */

#include "koho.local.h"

/*
 *
 */
string
Model::train(vector<Resident>& layout,
	     vector<mdreal>& history, const mdreal quota) {
  mdreal rlnan = medusa::rnan();
  ModelBuffer* p = (ModelBuffer*)buffer;
  map<string, Point>& points = p->points;
  mt19937& twister = p->twister;
  time_t stamp = time(NULL);
  
  /* Clear output containers. */
  layout.clear();
  history.clear();

  /* Check resources. */
  Topology topocopy = p->topology;
  mdsize npoints = (p->points).size();
  if(topocopy.size() < 1) return "No map units.";
  if(npoints < 10) return "Too few points.";
  if(p->ntrain < 10) return "Too few training points.";

  /* Final neighborhood radius for trained map. */
  mdreal sigma = (p->topology).sigma();

  /* Final inertia coefficient. */
  mdreal inertia = (sqrt(npoints) - sqrt(p->ntrain))/sqrt(npoints);
  
  /* Set initial model state. */
  map<string, mdreal>& state = p->state;
  if(state.count("sigma") < 1)
    state["sigma"] = 0.5*(topocopy.radius());
  if(state.count("inertia") < 1) {
    if(topocopy.radius() <= 0.0) state["inertia"] = inertia;
    else state["inertia"] = 0.0;
  }
  
  /* Create training engine. */
  Trainer trainer(p->codebook, topocopy, p->ntrain, p->equality);
  
  /* Make pointers to points. */
  vector<Point*> pointers;
  for(map<string, Point>::iterator it = points.begin();
      it != points.end(); it++)
    pointers.push_back(&(it->second));
  for(mdsize i = 0; i < pointers.size(); i++) {
    mdsize rank = twister()%npoints;
    Point* ptr = pointers[i];
    pointers[i] = pointers[rank];
    pointers[rank] = ptr;
  }
  
  /* Prepare sampling mask. */
  vector<Point*> pntmask = pointers;
  if(p->ntrain < npoints) pntmask.resize(p->ntrain);
  
  /* Fit codebook to training data. */
  bool timeout = false;
  vector<mdreal>& trace = p->trace;
  while((state["sigma"] >= 0.0) && !timeout) {

    /* Set neighborhood radius. */
    topocopy.rewire(state["sigma"]);

    /* Run training batch. */
    mdsize incr = 0;
    bool finished = false;
    while(!finished && !timeout) {
      
      /* Random cycling of the sampling mask. */
      if(pntmask.size() < npoints) {
	mdsize gap = twister()%((p->ntrain)/2 + 1);
	incr = (incr + p->ntrain/2 + gap)%npoints;
	for(mdsize i = 0; i < pntmask.size(); i++)
	  pntmask[i] = pointers[(i + incr)%npoints];
      }

      /* Perform a training cycle. */
      mdreal delta = trainer.match(pntmask, topocopy);
      trainer.update(topocopy, state["inertia"]);
      
      /* Check if initial centroids were available. */
      if(delta == rlnan) {
        if(trace.size() > 0) return "Training cycle failed.";
        if(history.size() < 1) {
          delta = trainer.match(pntmask, topocopy);
          trainer.update(topocopy, state["inertia"]);
        }
      }

      /* Store training error. */
      history.push_back(delta);
      trace.push_back(delta);
      finished = convergence(trace, 0.01);

      /* Check time quota. */
      if(quota != rlnan)
	timeout = (difftime(time(NULL), stamp) >= quota);
    }

    /* Check if batch was finished. */
    if(finished) {
      trace.clear();
 
      /* Check if training is finished. */
      if(state["sigma"] <= sigma) {
	state["sigma"] = -1.0;
	break;
      }
      
      /* Update neighborhood radius. */
      state["sigma"] *= 0.67;
      if(state["sigma"] < sigma) state["sigma"] = sigma;
      
      /* Update inertia. */
      state["inertia"] = exp(sigma - state["sigma"])*inertia;
    }
  }
  
  /* Update codebook. */
  p->codebook = trainer.codebook();

  /* Set final layout. */
  if(state["sigma"] < 0.0) {
    trainer = Trainer(p->codebook, topocopy,
		      pointers.size(), p->equality);
    mdreal delta = trainer.match(pointers, topocopy);
    if(history.size() > 0) history.push_back(delta);
  }

  /* Return final layout. */
  for(map<string, Point>::iterator it = points.begin();
      it != points.end(); it++) {
    Resident res;
    res.identity = it->first;
    res.district = (it->second).location();
    res.residual = trainer.distance(it->second, res.district);
    layout.push_back(res);
  }
  return "";
}
