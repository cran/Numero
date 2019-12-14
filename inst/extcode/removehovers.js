
// Clear all hover elements.
function removeHovers() {
    for(var i = 0; Hovers.length > i; i++) {
	var parent = Hovers[i].parentElement;
	var blocked = Hovers[i].getAttribute("blocked");
        parent.removeChild(Hovers[i]);
	if(!blocked) continue;
	var label = document.getElementById(blocked);
	label.setAttribute("visibility", "visible");
    }
    Hovers = [];
}
