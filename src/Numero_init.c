#include <R.h>
#include <Rinternals.h>
#include <R_ext/Rdynload.h>

SEXP nro_circus(SEXP offsets_R, SEXP topo_R, SEXP subcoldata_R, SEXP sublabdata_R, SEXP colordata_R, SEXP labels_R, SEXP txt_R, SEXP stamp_R);
SEXP nro_colorize(SEXP name_R);
SEXP nro_diffuse(SEXP topo_R, SEXP bmus_R, SEXP data_R);
SEXP nro_figure(SEXP fname_R, SEXP data_R, SEXP bbox_R);
SEXP nro_kohonen(SEXP seeds_R, SEXP rho_R);
SEXP nro_label(SEXP topo_R, SEXP data_R, SEXP binflags_R, SEXP sigma_R);
SEXP nro_match(SEXP codebook_R, SEXP data_R);
SEXP nro_permute(SEXP topo_R, SEXP bmus_R, SEXP data_R, SEXP numcycl_R);
SEXP nro_train(SEXP topo_R, SEXP codebook_R, SEXP data_R, SEXP nsub_R, SEXP eq_R);

R_CallMethodDef callMethods[]  = {
  {"nro_circus", (DL_FUNC) &nro_circus, 8},
  {"nro_colorize", (DL_FUNC) &nro_colorize, 1},
  {"nro_diffuse", (DL_FUNC) &nro_diffuse, 3},
  {"nro_figure", (DL_FUNC) &nro_figure, 3},
  {"nro_kohonen", (DL_FUNC) &nro_kohonen, 2},
  {"nro_label", (DL_FUNC) &nro_label, 4},
  {"nro_match", (DL_FUNC) &nro_match, 2},
  {"nro_permute", (DL_FUNC) &nro_permute, 4},
  {"nro_train", (DL_FUNC) &nro_train, 5},
  {NULL, NULL, 0}
};

void R_init_Numero(DllInfo *info) {
  R_registerRoutines(info, NULL, callMethods, NULL, NULL);
  R_useDynamicSymbols(info, FALSE);
}
