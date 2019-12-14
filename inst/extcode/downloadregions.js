
// Respond to clicking on a district.
function downloadRegions() {

    // Column headings.
    var colnames = Object.keys(TOPOLOGY);
    var data = colnames[0];
    for(var j = 1; colnames.length > j; j++)
	data += ("\t" + colnames[j]);
    data += "\n";

    // Copy topology.
    var topo = {};
    for(var j = 0; colnames.length > j; j++) {
	var c = colnames[j];
	topo[c] = TOPOLOGY[c].slice(0);
    }

    // Collect membership data.
    for(var i = 0; NDISTRICTS > i; i++) {
	var rkey = Activity.history[i].current;
	var detl = document.getElementById("detail_" + rkey);
	topo["REGION"][i] = "not_selected";
	topo["REGION.label"][i] = "";
	topo["REGION.color"][i] = "";
	if(1 > rkey.length) continue;
	topo["REGION"][i] = detl.textContent;
	topo["REGION.label"][i] = rkey;
	topo["REGION.color"][i] = REGIONCOLORS[rkey];
    }
   
    // Column values.
    for(var i = 0; NDISTRICTS > i; i++) {
        data += topo[colnames[0]][i];
	for(var j = 1; colnames.length > j; j++)
	    data += ("\t" + topo[colnames[j]][i]);
	data += "\n";
    }
    
    // Prepare output object.
    var output = new Blob([data], {type: 'text/plain'});
    var url = window.URL.createObjectURL(output);

    // Trigger download via a hyperlink.
    var link = document.getElementById("download_data");
    link.setAttribute("href", url);   
    link.click();

    // Release data object.
    window.URL.revokeObjectURL(url);   
}
