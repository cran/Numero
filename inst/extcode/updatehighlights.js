
// Update region indicators on a district.
function updateHighlights(dst, rkey) {
    if(!dst) return;
    for(var j = 0; SUBPLOTS.length > j; j++) {
	var sub = SUBPLOTS[j];

	// Check if valid district.
	var elem = document.getElementById(sub + "_paint_" + dst.key);
	if(!elem) {
	    window.alert("updateDistrict: Unusable element.");
	    return;
	}

	// Find highlight elements.
        const hlkey1 = (sub + "_active_" + dst.key);
	const hlkey0 = (sub + "_inactive_" + dst.key);
	var hlight1 = document.getElementById(hlkey1);
	var hlight0 = document.getElementById(hlkey0);

        // Determine district location.
        var offset = locateSubplot(sub);
	var x = (offset.x + dst.dx);
	var y = (offset.y + dst.dy);
	
        // Remove previous highlights if region changed.
	var prev = getDistrictData(elem);
	if(prev.region != rkey) {
	    if(hlight1) offset.area.removeChild(hlight1);
	    if(hlight0) offset.area.removeChild(hlight0);
	    hlight1 = null;
	    hlight2 = null;
	}

	// Update district data.
	elem.setAttribute("region", rkey);
	
	// Check if district is labeled.
	var label = document.getElementById(sub + "_label_" + dst.key);
	if(1 > rkey.length) {
	    if(label) label.setAttribute("visibility", "visible");
	    continue;
	}

	// Hide label to prevent overlap with highlight.
	if(label) label.setAttribute("visibility", "hidden");	

	// Create new highlight assemblies.
	if(!hlight1 || !hlight0) {

            // Determine highlight type and color.
	    var outline = document.getElementById("symbol_halo_" + rkey);
            if(!outline) window.alert("updateHighlights: Missing symbol.");
	    var shape = outline.getAttribute("shape");
	
            // Create a new symbol assembly.
	    var r = 0.87*(offset.unit);
	    var symbol = createSymbol(x, y, r, shape, false);
	
	    // Set active halo style and attributes.
 	    symbol.halo.style["fill"] = outline.style["fill"];
	    symbol.halo.setAttribute("pointer-events", "none");

	    // Set label style and attributes.
	    symbol.label.textContent = rkey;
 	    symbol.label.setAttribute("pointer-events", "none");
	
	    // Create a group for active highlight elements.
            hlight1 = document.createElementNS(SVGNS, "g");
	    hlight1.appendChild(symbol.halo);
	    hlight1.appendChild(symbol.label);
	    hlight1.id = hlkey1;

	    // Set inactive marker style and attributes.
	    hlight0 = symbol.marker;
	    hlight0.style["fill"] = outline.style["fill"];
	    hlight0.setAttribute("pointer-events", "none");
	    hlight0.id = hlkey0;
	    
	    // Add highlight on the district.
	    offset.area.appendChild(hlight0);
	    offset.area.appendChild(hlight1);
	}
	    
	// Set visibility based on activity status.
	if(Activity.regions[rkey]) {
	    hlight1.style["visibility"] = "visible";
	    hlight0.style["visibility"] = "hidden";
	} else {
	    hlight1.style["visibility"] = "hidden";
	    hlight0.style["visibility"] = "visible";
	}
    }
}
