
// Respond to entering an element's area.
function pointerOverDistrict(evt) {
    
    // Check if pointer is down and over a district.
    Activity.hold = (evt.buttons != 0);
    if(Activity.hold) {
	pointerDownDistrict(evt);
	return;
    }

    // Remove existing hover elements.
    removeHovers();

    // Check if over a district.
    var dst = getDistrictData(evt.target);
    if(!dst) return;
    
    // Create new hover elements.
    for(var j = 0; SUBPLOTS.length > j; j++)
	createHoverDistrict(SUBPLOTS[j], dst.key, dst.dx, dst.dy);
}
