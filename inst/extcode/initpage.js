
// Launch widget.
function initPage(plot, guiFlag) {
    
    // Check that there is at least one plot.
    if(!SUBPLOTS) {
 	window.alert("initPage: Subplots not defined.");
	return;
    }
    if(1 > SUBPLOTS.length) {
 	window.alert("initPage: No subplots.");
	return;
    }
    
    // Determine window content offset.
    var conts = document.getElementById(plot + "_contents")
    var item0 = conts.transform.baseVal.getItem(0);
    if(item0.type == SVGTransform.SVG_TRANSFORM_TRANSLATE) {
        Origin.x = item0.matrix.e;
        Origin.y = item0.matrix.f;
    }
    
    // No interactive elements.
    if(!guiFlag) {
	document.addEventListener("pointerover", pointerOverDistrict, false);
	return;
    }

    // Clear any shadow elements.
    for(var j = 0; SUBPLOTS.length > j; j++) {
	var sGroup = document.getElementById(SUBPLOTS[j] + "_shadow");
	if(sGroup) sGroup.parentNode.removeChild(sGroup);
    }
    
    // Find calibration element.
    var id = (SUBPLOTS[0] + "_calibration");
    var calibr = document.getElementById(id);
    if(!calibr) {
 	window.alert("initPage: Calibration failed.");
	return;
    }

    // Tables to keep track of activity.
    Activity.hold = false;
    Activity.recent = "";
    Activity.regions = {};
    Activity.history = [];
    for(var i = 0; NDISTRICTS > i; i++)
	Activity.history.push({"current":"", "previous":""});
    
    // Set up membership matrix for regions.
    Membership = {};
    for(var k in REGIONS)
	Membership[k] = {"length":0};

    // Set distance unit.
    var bbox = calibr.getBoundingClientRect();
    Unit = bbox.width;

    // Set up subgroup selector and define symbol shapes
    // for regions, must be before initRegions().
    if(!initMenu()) {
 	window.alert("initPage: Menu failed.");
	return;
    }
    
    // Set up regions.
    if(!initRegions(TOPOLOGY)) {
 	window.alert("initRegions: Highlights failed.");
	return;
    }
    
    // Add content-specific event listeners.
    var bottom = document.getElementById("bottom");
    if(window.PointerEvent) {
	bottom.addEventListener("pointerover", pointerOverDistrict, false);
	bottom.addEventListener("pointerdown", pointerDownDistrict, false);
    }
    else {
	bottom.addEventListener("mouseover", pointerOverDistrict, false);
	bottom.addEventListener("mousedown", pointerDownDistrict, false);
    }
}
