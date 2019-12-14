
// Respond to clicking on a symbol area.
function pointerDownSymbol(evt) {
    
    // Check if valid target.
    var rkey = evt.target.getAttribute("key");
    if(!rkey) return;

    // Check activity status.
    var active = evt.target.getAttribute("active");
    if(!active) {
	window.alert("pointerDownSymbol: Unusable activity status.");
	return;
    }

    // Check that region is defined in the symbol menu.
    var target0 = document.getElementById("symbol_inactive_" + rkey);
    var target1 = document.getElementById("symbol_active_" + rkey);
    if(!target0 || !target1) {
	window.alert("pointerDownSymbol: Region not defined.");
	return;
    }
    
    // Activate a region that is currently inactive.
    if(active == "false") {
	target0.style["visibility"] = "hidden";
	target1.style["visibility"] = "visible";
	Activity.regions[rkey] = true;
    }

    // De-activate a region that is currently active.
    if(active == "true") {
	target0.style["visibility"] = "visible";
	target1.style["visibility"] = "hidden";
	Activity.regions[rkey] = false;
    }
    
    // Update districts.
    var memb = Membership[rkey];
    for(k in memb) {
	var dst = getDistrictData(memb[k]);
        updateHighlights(dst, rkey);
    }
}
