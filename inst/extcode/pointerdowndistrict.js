
// Respond to clicking on a district.
function pointerDownDistrict(evt) {
    
    // Clear any hover elements.
    removeHovers();

    // Check if over a district.
    var dst = getDistrictData(evt.target);
    if(!dst) {
	if(Activity.hold) return;
	if(window.confirm("Download results?"))
	    downloadRegions();
	return;
    }

    // Check that there is a single active region.
    var nregs = 0;
    var rkey = "";
    for(var k in Activity.regions) {
	if(Activity.regions[k]) {
	    rkey = k;
 	    nregs++;
	}
	if(nregs > 1) {
	    window.alert("Please select a single region.");
	    return;
	}
    }
    if(1 > nregs) {
	window.alert("Please select a region first.");
	return;
    }
    
    // Current and previous assignments.
    var history = Activity.history[dst.key];

    // Determine replacement region.
    if(Activity.hold) {

        // Check if clearance is needed.
	if(1 > Activity.recent.length) {
	    if(history.current != rkey) return;
	    rkey = history.previous;
	}
	else {
	    rkey = Activity.recent;
	}
	
	// Check if already up-to-date.
	if(history.current == rkey) return;
    }

    // Remove current assignment.
    if(history.current.length > 0) {
	Membership[history.current][dst.key] = null;
	Membership[history.current].length -= 1;
    }
	
    // If repeated click, restore previous assignment.
    // If new click, update assignment.
    if(history.current == rkey) {
	history.current = history.previous;
	history.previous = "";
	if(!Activity.hold) Activity.recent = "";
    }
    else {
	history.previous = history.current;
	history.current = rkey;
        if(!Activity.hold) Activity.recent = rkey;
    }
    
    // Set new assignment.
    if(history.current.length > 0) {
	Membership[history.current][dst.key] = evt.target;
	Membership[history.current].length += 1;
    }

    // Update visuals.
    updateHighlights(dst, history.current);
 }
