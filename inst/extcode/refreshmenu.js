
// Document-wide subgroup activation.
function refreshMenu() {
    const fontSize = 0.55*Unit;

    // Check that menu is available.
    var detl = document.getElementById("detail");
    if(!detl) {
	window.alert("refreshMenu: Details not defined.");
	return false;
    }
    
    // Make sure text is up-to-date.
    var reserved = {};
    for(k in REGIONS) {
	var txt = document.getElementById("detail_" + k);

	// Create a text element for region detail.
	if(!txt) {
	    var label = document.getElementById("symbol_label_" + k);
	    var halo = document.getElementById("symbol_halo_" + k);
            var halobox = halo.getBoundingClientRect();
	    var y = (0.5*(halobox.top + halobox.bottom) - 0.25*fontSize);
	    txt = document.createElementNS(SVGNS, "text");
	    txt.style["fill"] = "#a0a0a0";
            txt.style["font-family"] = label.style["font-family"];
            txt.style["font-size"] = (Math.round(fontSize) + "px");
            txt.setAttribute("x", 0);
            txt.setAttribute("y", y);
            txt.setAttribute("key", k);
            txt.id = ("detail_" + k);
	    detl.appendChild(txt);
	}

        // Check if first refresh.
	if(!MenuTimer) txt.textContent = REGIONS[k];
	
	// Make sure name is usable.
	while(2 > txt.textContent.length) {
	    var msg = ("Please enter a name for Region " + k + ":");
	    removeHovers(); // hovers may get stuck after prompt
	    txt.textContent = window.prompt(msg, "");
	}
	
	// Include region name in the lookup table.
	reserved[txt.textContent] = k;
    }

    // Update text style according to memberships and activity.
    for(k in REGIONS) {
	var txt = document.getElementById("detail_" + k);
	var memb = Membership[k];
	if(Activity.regions[k] == true) {
	    if(memb.length > 0) {
		txt.style["font-weight"] = "bold";
		txt.style["fill"] = "#000000a0";
	    }
	    else {
		txt.style["font-weight"] = "normal";
		txt.style["fill"] = "#00000080";
	    }
	}
	else {
	    if(memb.length > 0) {
		txt.style["font-weight"] = "bold";
		txt.style["fill"] = "#505050";
	    }
	    else {
		txt.style["font-weight"] = "normal";
		txt.style["fill"] = "#a0a0a0";
	    }
	}

	// Check if the name is duplicated.
	if(reserved[txt.textContent] != k)
	    txt.style["fill"] = "#fa1010";
	reserved[txt.textContent] = k;
    }
}
