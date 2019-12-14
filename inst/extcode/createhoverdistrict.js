
// Create a new hover element assembly.
function createHoverDistrict(plot, key, dx, dy) {

    // Find target element.
    var elem = document.getElementById(plot + "_paint_" + key);
    if(!elem) {
	window.alert("District not found.")
	return;
    }
    
    // Find calibration element.
    var calibr = document.getElementById(plot + "_calibration");
    if(!calibr) {
	window.alert("Calibration failed.")
	return;
    }

    // Find the drawing area subject to translation.
    var area = document.getElementById(plot + "_contents");
    if(!area) area = document.getElementById("plot_contents");
    if(!area) {
	window.alert("Cannot determine plot contents.");
	return;
    }

    // Find the SVG element within the document.
    var svg = document.getElementById(plot);
    if(!svg) svg = document.getElementById("plot");
    if(!svg) {
	window.alert("Cannot determine svg element.");
	return;
    }

    // Determine scroll offset.
    var bbox = svg.getBoundingClientRect();
    var scroll = {"x":bbox.x, "y":bbox.y};

    // Determine translation of main contents.
    var transl = {"x":area.getAttribute("tfx"),
		  "y":area.getAttribute("tfy")};
    
    // Determine target location.
    var offset = calibr.getBoundingClientRect();
    var x = (dx + offset.x - transl.x - scroll.x);
    var y = (dy + offset.y - transl.y - scroll.y);
    var unit = offset.width;

    // New element for district highlight.
    var clon = elem.cloneNode(false);
    clon.setAttribute("pointer-events", "none");
    clon.style["fill"] = "none";
    clon.style["stroke"] = HOVERCOLOR;
    clon.style["stroke-width"] = "2px";
    clon.style["stroke-linejoin"] = "round";

    // New element for info text.
    var txt = document.createElementNS(SVGNS, "text");
    txt.setAttribute("pointer-events", "none");
    txt.style["font-family"] = "'Arial'";
    txt.style["font-size"] = (Math.round(0.5*unit) + "px");
    txt.style["font-weight"] = "bold";
    txt.style["text-anchor"] = "middle";
    txt.style["fill"] = "white";

    // New element for text background.
    var bg = document.createElementNS(SVGNS, "rect");
    bg.setAttribute("rx", 0.1*unit);
    bg.setAttribute("ry", 0.1*unit);
    bg.setAttribute("pointer-events", "none");
    bg.style["fill"] = HOVERCOLOR;
    bg.style["stroke"] = "none";

    // Set text contents.
    txt.textContent = LABELS[plot][key];

    // Relative position to map center in screen coordinates.
    var dx = (elem.getAttribute("v2"))*unit;
    var dy = (elem.getAttribute("v3"))*unit;
    
    // Set text position.
    txt.setAttribute("x", x);
    txt.setAttribute("y", (y - 0.5*unit));
    
    // Check if district is labeled.
    var label = document.getElementById(plot + "_label_" + key);
    if(label) {
	if(label.getAttribute("visibility") != "hidden") {
	    clon.setAttribute("blocked", label.id);
	    label.setAttribute("visibility", "hidden");
	}
    }

    // Add to parent element.
    area.appendChild(clon);
    area.appendChild(bg);
    area.appendChild(txt);
    
    // Determine text dimensions.
    var tbox = txt.getBoundingClientRect();
    var w = (tbox.width + 0.3*unit);
    var h = (tbox.height + 0.1*unit);
    
    // Finish background box.
    bg.setAttribute("x", (x - 0.5*w));
    bg.setAttribute("y", (y - 0.67*unit - 0.5*h));
    bg.setAttribute("width", w);
    bg.setAttribute("height", h);

    // Add elements to global vector.
    Hovers.push(clon);
    Hovers.push(bg);
    Hovers.push(txt);
}
