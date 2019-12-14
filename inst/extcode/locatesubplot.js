
// Location and scale of a subplot.
function locateSubplot(sub) {

    // Find the drawing area subject to translation.
    var area = document.getElementById(sub + "_contents");
    if(!area) area = document.getElementById("plot_contents");
    if(!area) {
	window.alert("locateSubplot: Cannot find contents.");
	return null;
    }
	
    // Find the SVG element within the document.
    var svg = document.getElementById(sub);
    if(!svg) svg = document.getElementById("plot");
    if(!svg) {
	window.alert("locateSubplot: Cannot find svg element.");
	return null;
    }

    // Find calibration element.
    var calibr = document.getElementById(sub + "_calibration");
    if(!calibr) calibr = document.getElementById("plot_calibration");
    if(!calibr) {
	window.alert("locateSubplot: Calibration failed.");
	return null;
    }
	
    // Determine scroll offset.
    var bbox = svg.getBoundingClientRect();
    var scroll = {"x":bbox.x, "y":bbox.y};
	
    // Determine translation of main contents.
    var transl = {"x":parseFloat(area.getAttribute("tfx")),
		  "y":parseFloat(area.getAttribute("tfy"))};

    // Finish calibration box.
    var offset = calibr.getBoundingClientRect();
    offset.x -= (transl.x + scroll.x);
    offset.y -= (transl.y + scroll.y);

    // Return results.
    var output = {};
    output.x = offset.x;
    output.y = offset.y;
    output.unit = offset.width;
    output.area = area;
    return output;
}
