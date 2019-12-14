
// Create menu items for subgroup selection.
function initMenu() {

    // Check that regions have been defined.
    if(!REGIONS) {
 	window.alert("initMenu: Regions not defined.");
	return false;
    }
    
    // Additional modifiers.
    const fontSize = 0.47*Unit;
    const textWidth = DETAILWIDTH*fontSize;
    const lineWidth = 0.08*Unit;
  
    // Determine menu block size.
    const width = (textWidth + (1.0 + 0.5)*Unit);
    const height = (Object.keys(REGIONS).length)*Unit;
 
    // Adjust the hovering side block that contains the menu.
    var side = document.getElementById("side");
    side.style.width = (width + "px");
    side.style.height = (height + "px");

    // Top menu layer with editable text and a gap for
    // interacting with symbols on the layer below.
    var top = document.getElementById("top");
    top.style.marginLeft = (1.33*Unit + "px");
  
    // SVG block that contains subgroup details.
    var detl = document.getElementById("detail");
    detl.setAttribute("width", width);
    detl.setAttribute("height", height);

    // SVG block that contains symbols and the background.
    var sym = document.getElementById("symbol");
    sym.setAttribute("width", width);
    sym.setAttribute("height", height);

    // Menu background.
    var menubg = document.getElementById("symbol_background");
    menubg.setAttribute("x", 0);
    menubg.setAttribute("y", 0);
    menubg.setAttribute("width", width);
    menubg.setAttribute("height", height);
    menubg.setAttribute("rx", 0.2*Unit);
    menubg.setAttribute("ry", 0.2*Unit);
    menubg.style["fill"] = "#ffffffa0";
    menubg.style["stroke"] = "none";
 
    // Create menu items.
    var baseline = 0;
    for(var k in REGIONS) {
	var x = (0.5*Unit + lineWidth);
	var y = (baseline + 0.5)*Unit;
	var r = (0.5*Unit - lineWidth);

	// Create a group for elements in activated highlight.
        var hlight = document.createElementNS(SVGNS, "g");
	hlight.id = ("symbol_active_" + k);
	
	// Background for activated detail text area.
	var txtbg = document.createElementNS(SVGNS, "rect");
	txtbg.style["fill"] = REGIONCOLORS[k];
	txtbg.style["fill-opacity"] = 0.4;
	txtbg.setAttribute("pointer-events", "none");
	txtbg.setAttribute("x", x);
	txtbg.setAttribute("y", (y - r + 0.81*lineWidth));
	txtbg.setAttribute("width", (textWidth + 0.2*Unit));
	txtbg.setAttribute("height", 2*(r - 0.81*lineWidth));
	txtbg.setAttribute("rx", 0.2*r);
	txtbg.setAttribute("ry", 0.2*r);
	
        // Add background to highlight group.
	hlight.appendChild(txtbg);

        // Create symbol assembly.
	var shape = "circle";
	if(baseline%3 == 1) shape = "square";
	if(baseline%3 == 2) shape = "diamond";
	var elems = createSymbol(x, y, Unit, shape, true);

	// Set paint color.
	elems.halo.style["fill"] = REGIONCOLORS[k];
 	elems.marker.style["fill"] = REGIONCOLORS[k];

	// Set region identity.
	elems.halo.setAttribute("key", k);
 	elems.marker.setAttribute("key", k);
	elems.label.setAttribute("key", k);
	elems.label.textContent = k;
	
        // Background to prevent text box from
	// showing through transparent halo.
	var halobg = elems.halo.cloneNode(false);
	halobg.setAttribute("pointer-events", "none");
	halobg.style["fill"] = halobg.style["stroke"]

	// Set element identities.
	elems.halo.id = ("symbol_halo_" + k);
 	elems.label.id = ("symbol_label_" + k);
	elems.marker.id = ("symbol_inactive_" + k);
	halobg.id = ("symbol_background_" + k);
	
        // Add highlight elements to group.
	hlight.appendChild(halobg);
	hlight.appendChild(elems.halo);
	hlight.appendChild(elems.label);
	
	// Set initial activity status.
	hlight.style["visibility"] = "hidden";
	elems.marker.style["visibility"] = "visible";

        // Add activity items to the symbol block.
        sym.appendChild(hlight);
        sym.appendChild(elems.marker);
        baseline++;
    }
        
    // Add menu-specific event listeners.
    var middle = document.getElementById("middle");
    if(window.PointerEvent) {
	middle.addEventListener("pointerover", pointerOverSymbol, false); 
	middle.addEventListener("pointerdown", pointerDownSymbol, false);
    }
    else {
	middle.addEventListener("mouseover", pointerOverSymbol, false); 
	middle.addEventListener("mousedown", pointerDownSymbol, false);
    }
    
    // Launch background refresh for updates.
    if(!MenuTimer) {
	refreshMenu(); // initial set up
	MenuTimer = window.setInterval(refreshMenu, 100);
    }
    return true;
}
