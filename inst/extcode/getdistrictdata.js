
// Collect district-specific data.
function getDistrictData(elem) {
    if(!elem) return null;
    if((typeof elem) != "object") return null;
    
    // Extract data fields.
    var subplot = elem.getAttribute("v0"); 
    var key = elem.getAttribute("v1");
    var dx = elem.getAttribute("v2");
    var dy = elem.getAttribute("v3");
    if(!subplot) return null;
    if(!key) return null;
    if(!dx) return null;
    if(!dy) return null;

    // Determine activity status and region assignment.
    var active = elem.getAttribute("active"); 
    var region = elem.getAttribute("region"); 
    active = (active == "true");
    
    // Finish results.
    output = {};
    output.subplot = subplot;
    output.key = parseInt(key);
    output.dx = parseFloat(dx);
    output.dy = parseFloat(dy);
    output.active = active;
    output.region = region;
    return output;
}
