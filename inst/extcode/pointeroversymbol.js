
// Respond to pointer within a menu symbol.
function pointerOverSymbol(evt) {
    
    // Remove any previous hover elements.
    removeHovers();

    // Check if pointer is down.
    if(evt.buttons != 0) return;
    
    // Find the active version of the symbol.
    var key = evt.target.getAttribute("key");
    var elem = document.getElementById("symbol_halo_" + key);
    if(!elem) return;
   
    // Create a new hover element.
    var halo = elem.cloneNode(false);
    var lw = parseFloat(halo.style["stroke-width"])
    halo.style["stroke-width"] = lw;
    halo.style["stroke"] = HOVERCOLOR;
    halo.style["fill"] = "none";
    halo.setAttribute("pointer-events", "none");
    
    // Add element to the figure.
    evt.target.parentElement.appendChild(halo);
    Hovers.push(halo);
}
