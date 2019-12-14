
// Create elements for a symbol assembly.
function createSymbol(x, y, unit, shape, strokeFlag) {

    // Additional modifiers.
    var fontSize = 0.47*unit;
    var strokeWidth = 0.08*unit;
    var radius1 = (0.45*unit - strokeWidth);
    var radius0 = (radius1 - 2.3*strokeWidth);

    // Adjust radius if reduced stroke.
    if(!strokeFlag) radius1 -= 0.3*strokeWidth;
    
    // Halo around activated label and marker for inactivity.
    var halo = undefined;
    var marker = undefined;
    if(shape == "circle") {
	halo = document.createElementNS(SVGNS, "circle");
	halo.setAttribute("cx", x);
	halo.setAttribute("cy", y);
	halo.setAttribute("r", radius1);
	marker = document.createElementNS(SVGNS, "circle");
	marker.setAttribute("cx", x);
	marker.setAttribute("cy", y);
	marker.setAttribute("r", radius0)
    }
    if(shape == "square") {
	halo = document.createElementNS(SVGNS, "rect");
	halo.setAttribute("x", (x - 0.93*radius1));
	halo.setAttribute("y", (y - 0.93*radius1));
	halo.setAttribute("width", 1.86*radius1);
	halo.setAttribute("height", 1.86*radius1);
	marker = document.createElementNS(SVGNS, "rect");
	marker.setAttribute("x", (x - 0.92*radius0));
	marker.setAttribute("y", (y - 0.92*radius0));
	marker.setAttribute("width", 1.84*radius0);
	marker.setAttribute("height", 1.84*radius0);
    }
    if(shape == "diamond") {
	var xy1 = (String(x - 1.2*radius1) + "," + String(y));
	xy1 += (" " + String(x) + "," + String(y + 1.25*radius1));
	xy1 += (" " + String(x + 1.2*radius1) + "," + String(y));
	xy1 += (" " + String(x) + "," + String(y - 1.25*radius1));
	halo = document.createElementNS(SVGNS, "polygon");
	halo.setAttribute("points", xy1);
	var xy0 = (String(x - 1.2*radius0) + "," + String(y));
	xy0 += (" " + String(x) + "," + String(y + 1.3*radius0));
	xy0 += (" " + String(x + 1.2*radius0) + "," + String(y));
	xy0 += (" " + String(x) + "," + String(y - 1.3*radius0));
	marker = document.createElementNS(SVGNS, "polygon");
	marker.setAttribute("points", xy0);
    }
    if(!halo) {
	window.alert("createSymbol: Unknown shape.");
	return null;
    }
    
    // Set halo style and attributes.
    halo.style["stroke"] = "#ffffff";
    if(strokeFlag) halo.style["stroke-width"] = strokeWidth;
    else halo.style["stroke-width"] = 0.6*strokeWidth;
    halo.setAttribute("active", true);
    halo.setAttribute("shape", shape);
	
    // Activated region label.
    var label = document.createElementNS(SVGNS, "text");
    label.style["font-family"] = "'Arial'";
    label.style["fill"] = "#ffffff";
    label.style["font-weight"] = "bold";
    label.style["font-size"] = (Math.round(fontSize) + "px");
    label.style["text-anchor"] = "middle";
    label.setAttribute("pointer-events", "none");
    label.setAttribute("x", x);
    label.setAttribute("y", (y + 0.33*fontSize));
    label.setAttribute("yoffset", 0.33*fontSize);
    label.setAttribute("active", true);

    // Set marker style and attributes.
    marker.style["stroke"] = "#ffffff";
    marker.style["stroke-width"] = 0.6*strokeWidth;
    marker.setAttribute("active", false);
    marker.setAttribute("shape", shape);
    
    // Return results.
    return {"halo":halo, "label":label, "marker":marker};
}
