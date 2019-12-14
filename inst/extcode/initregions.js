
// Set up region elements.
function initRegions(topology) {

    // Clear any existing highlights.
    for(var j = 0; SUBPLOTS.length > j; j++) {
	var sGroup = document.getElementById(SUBPLOTS[j] + "_region");
        if(sGroup) sGroup.parentNode.removeChild(sGroup);
    }
    
    // Create highlights according to topology.
    for(var i = 0; NDISTRICTS > i; i++) {
        var rkey = topology["REGION.label"][i];
	if(1 > rkey.length) continue;
	 
        // District details.
        var key = (SUBPLOTS[0] + "_paint_" + i);
	var target = document.getElementById(key)
        var dst = getDistrictData(target);
	var history = Activity.history[dst.key];

        // Update membership.
        history.current = rkey;
	Membership[history.current][i] = target;
	Membership[history.current].length += 1;
        updateHighlights(dst, history.current);
    }
    return true;
}
