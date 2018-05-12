
inlets = 1;
outlets = 16;
// 0   : gate for functions when no devices
// 1-8 : devices
// 9   : awake devices
// 10  : asleep devices
// 11  : strings to message box
// 12  : strings to send via OSC
// 13  : active device led indicator controller
// 14  : request setting response 
// 15  : set dropdown

d = new Dict("Channels");
var json;

function bang()
{

}

// clear all devices and reload from json
function reset()
{
	outlet(11, "Clearing stored devices");
	d.clear();
	for (i=0; i<8; i++) {
		outlet(i+1, "textcolor", 0., 0., 0., 0.4);	
		outlet(13, 0, i+1);
	}
	
	d.import_json("Channels.json");
	var str = d.stringify();
	json = JSON.parse(str);
	outlet(15, 0);
	update_awake();
	display();
}

function refresh1()
{
	for (i=0; i<8; i++) {
		json.devices[i].awake = 0;
	}
	outlet(11, "Refreshing list of devices");
	outlet(12, "/LOOM/ChannelPoll");
	//outlet(12, "bang");
	outlet(0, 0);
	outlet(9, "clear");
	outlet(10, "clear");
}

function refresh2()
{
	display();
}

function display()
{
	for (i=0; i<8; i++) {
		outlet(i+1, "set", json.devices[i].type);
		if (json.devices[i].awake == 1) {
			outlet(i+1, "textcolor", 0., 0., 0., 1.);
			outlet(13, 1, i+1);
		} else {
			outlet(i+1, "textcolor", 0., 0., 0., 0.4);
			outlet(13, 0, i+1);
		}
	}
}

function update_awake()
{
	outlet(0, 0);
	outlet(9, "clear");
	outlet(10, "clear");
	for (i=0; i<8; i++) {
		var tmp = json.devices[i];
		if (tmp.awake == 1) {
			outlet(9, "append", tmp.letter);
			outlet(0, 1);
		} 
	}
	if (json.reassign == 1) {
		update_free();
	}
	display();
}

function update_free()
{
	outlet(10, "clear");
	for (i=0; i<8; i++) {
		var tmp = json.devices[i];
		if (tmp.free == 1) {
			outlet(10, "append", tmp.letter);
		} 
	}
}

function device_poll(t, i)
{
	json.devices[i-1].type = t;
	json.devices[i-1].awake = 1;
	json.devices[i-1].free = 0;
	var str = "Scan found " + t + " on channel " + json.devices[i-1].letter;
	outlet(11, str);
	update_awake();
}
	
function device_request(t, c)
{
	for (i=0; i<8; i++) {
		if (json.devices[i].free) 
		{
			json.devices[i].free = 0;
			json.devices[i].type = t;
			json.devices[i].awake = 1;
			
			var str = "New " + t + " requested channel, assigned to " + json.devices[i].letter;
			outlet(11, str);
			str = "/LOOM/" + json.devices[i].type + c + "/SetPort";
			outlet(14, str, 9441+i);
			str = "/LOOM/" + json.devices[i].type + c + "/SetID";
			outlet(14, str, i+1);
			//outlet(12, "bang");
			update_awake();
			return;
		}
	}
	var str = "No channel available for " + t;
	outlet(11, str);
}

function save(i)
{
	var str = "Saved Config of " + json.devices[i-1].type + " on channel " + json.devices[i-1].letter;
	outlet(11, str);
	str = "/LOOM/" + json.devices[i-1].type + i + "/SaveConfig";
	outlet(12, str);
	//outlet(12, "bang");
}

function set(i)
{
	var str = "Set request settings for " + json.devices[i-1].type + " on channel " + json.devices[i-1].letter;
	outlet(11, str);
	str = "/LOOM/" + json.devices[i-1].type + i + "/SetRequestSettings";
	outlet(12, str);
	//outlet(12, "bang");
}

function reassign(from, to)
{
	var str = "Reassigning " + json.devices[from-1].type + " on channel " + json.devices[from-1].letter + " to " + json.devices[to-1].letter;
	outlet(11, str);
	str = "/LOOM/" + json.devices[from-1].type + from + "/SetPort";
	outlet(12, str, 9440+to);
	str = "/LOOM/" + json.devices[from-1].type + from + "/SetID";
	outlet(12, str, to);
	//outlet(12, "bang");
	
	json.devices[to-1].type = json.devices[from-1].type;
	json.devices[to-1].free = 0;
	json.devices[to-1].awake = 1;
	json.devices[from-1].type = "";
	json.devices[from-1].free = 1;
	json.devices[from-1].awake = 0;
	
	update_awake();
}

function remove(i)
{
	var str = "Removed " + json.devices[i-1].type + " on channel " + json.devices[i-1].letter;
	outlet(11, str);
	str = "/LOOM/" + json.devices[i-1].type + i + "/wifiSetup/AP";
	outlet(12, str);
	
	json.devices[i-1].free = 1;
	json.devices[i-1].type = "";
	json.devices[i-1].awake = 0;
	
	outlet(13, 0, i);
	
	update_awake();
}


function transmitting(i)
{
	if (json.devices[i-1].free == 0) {
		json.devices[i-1].awake = 1;
		outlet(i, "textcolor", 0., 0., 0., 1.);
		outlet(13, 1, i);
	}
}

function set_reassign(e)
{
	json.reassign = e;
}