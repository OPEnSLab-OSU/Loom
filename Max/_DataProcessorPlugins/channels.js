
inlets = 1;
outlets = 10;
// 0  : gate for functions when no devices
// 1  : device display
// 2  : awake devices
// 3  : asleep devices
// 4  : strings to message box
// 5  : strings to send via OSC
// 6  : set UDP ports
// 7  : set subnet routing 
// 8  : enable individual battery and button displays
// 9  : device targeting output

// Current state of the devices on the channels
var json = {
	"devices" : [ 		
		{ "free" : 1, "type" : "", "letter" : "A", "awake" : 0 },	
		{ "free" : 1, "type" : "", "letter" : "B", "awake" : 0 },	
		{ "free" : 1, "type" : "", "letter" : "C", "awake" : 0 },	
		{ "free" : 1, "type" : "", "letter" : "D", "awake" : 0 },	
		{ "free" : 1, "type" : "", "letter" : "E", "awake" : 0 },	
		{ "free" : 1, "type" : "", "letter" : "F", "awake" : 0 },	
		{ "free" : 1, "type" : "", "letter" : "G", "awake" : 0 },	
		{ "free" : 1, "type" : "", "letter" : "H", "awake" : 0 }
	 ],
	"reassign" : 0
}

var global_port = 9400;

// Current subnet to manage
var family = "LOOM";
var family_num = 1;
var subnet_prefix = '/' + family + family_num;

// Currently no functionality implemented upon bang
function bang()
{

}


// Check if value is in range
function verify(i, l, h) 
{ 
	return ((i >= l) && (i <= h)); 
}

// New family name was set
function new_family(f) {
	family = f;
	subnet_prefix = '/' + family + family_num;
	reset();
	refresh();
}

// New family number (subnet) was set
function new_family_num(i) {
	if (verify(i, 0, 12)) {
		family_num = i;
		subnet_prefix = '/' + family + family_num;
	}
	else outlet(4, i + ' is not a valid family number');
	reset();
	refresh();
}


// Clear all devices and reload blank data structure from Channels.json
function reset()
{
	outlet(4, 'Clearing stored devices');
	for (i=0; i<8; i++) {
		outlet(1, '/text', i, 'textcolor', 0., 0., 0., 0.4);	
		outlet(1, '/led', i, 0);
		json.devices[i].awake = 0;
		json.devices[i].free  = 1;
		json.devices[i].type  = "";
	}
	json.reassign = false;
	
	update_routing();
	update_udp();
	update_receive_gates();
	
	update_awake();
	display();
}


// Refreshes the known state of the devices
// Marks devices as asleep
// Sends out poll to devices to check which are awake
// Clears dynamic dropdown menus 
function refresh()
{
	for (i=0; i<8; i++) {
		json.devices[i].awake = 0;
	}
	outlet(4, 'Refreshing list of devices');
	outlet(5, subnet_prefix + '/ChannelPoll');
	outlet(0, 0);
	outlet(2, 'clear');
	outlet(3, 'clear');
	update_receive_gates();	
}


// For awake devices, displays 
//   Device name in full opacity 
//   Lights corresponding Led element
// For asleep devices, displays
//   Device name in 0.4 opacity
//   Turns off Led element
// Empty channels display nothing
function display()
{
	for (i=0; i<8; i++) {
		outlet(1, '/text', i+1, 'set', json.devices[i].type);
		if (json.devices[i].awake == 1) {
			outlet(1, '/text', i+1, 'textcolor', 0., 0., 0., 1.);
			outlet(1, '/led', i+1, 1);
			
			//outlet(9, i, family, family_num, json.devices[i].type, i+1);	
			//outlet(9, i+1, 0, family);
//			outlet(9, i+1, 1, family_num);
//			outlet(9, i+1, 2, json.devices[i].type);
//			outlet(9, i+1, 3, i+1);
			
		} else {
			outlet(1, '/text', i+1, 'textcolor', 0., 0., 0., 0.4);
			outlet(1, '/led', i+1, 0);
		}
	}
	
	for (i=0; i<8; i++) {
		// if (!json.devices[i].free) {
			outlet(9, i+1, 1, family_num);
			outlet(9, i+1, 2, json.devices[i].type);
			outlet(9, i+1, 3, i+1);
		// }
	}
}

// Updates the dropdown menu of selectable devices
// Calls display() to refresh display of current state
// Only calls update_free() if Reassign From .. To .. 
//    is the selected control option
function update_awake()
{
	outlet(0, 0);
	outlet(2, 'clear');
	outlet(3, 'clear');
	for (i=0; i<8; i++) {
		var tmp = json.devices[i];
		if (tmp.awake == 1) {
			outlet(2, 'append', tmp.letter);
			outlet(0, 1);
		} 
	}
	if (json.reassign == 1) {
		update_free();
	}
	display();
}

// Updates the second channel dropdown menu with channels
// that do not have any devices on them, awake or asleep
function update_free()
{
	outlet(3, 'clear');
	for (i=0; i<8; i++) {
		var tmp = json.devices[i];
		if (tmp.free == 1) {
			outlet(3, 'append', tmp.letter);
		} 
	}
}

// Device poll response has been routed into the js object
// Updates the corresponing channel (based on device inst.) with device name
// Updates dropdown of available devices
function device_poll(t, i)
{
	json.devices[i-1].type = t;
	json.devices[i-1].awake = 1;
	json.devices[i-1].free = 0;
	var str = 'Scan found ' + t + ' on channel ' + json.devices[i-1].letter;
	outlet(4, str);
	update_awake();
	update_receive_gates();
}
	
// Device has requested channel settings
// Iterate through channels to find first empty channel
// Assign device to this channel, immediately updating display
// Sends corresponding set Port and Instance # commands to device
// If no channels available, report to message box
function device_request(t, c)
{
	for (i=0; i<8; i++) {
		if (json.devices[i].free) 
		{
			json.devices[i].free = 0;
			json.devices[i].type = t;
			json.devices[i].awake = 1;
			
			var str = 'New ' + t + ' requested channel, assigned to ' + json.devices[i].letter;
			outlet(4, str);
			
			str = subnet_prefix + '/' + json.devices[i].type + c + "/SetChannel";
			outlet(5, str, i+1);
					
			update_awake();
			update_receive_gates();
			return;
		}
	}
	var str = 'No channel available for ' + t;
	outlet(4, str);
}


// Send command to specified device to save its 
// configuration struct to non-volatile memory
function save(i)
{
	var str = 'Saved Config of ' + json.devices[i-1].type + ' on channel ' + json.devices[i-1].letter;
	outlet(4, str);
	str = subnet_prefix + '/' + json.devices[i-1].type + i + '/SaveConfig';
	outlet(5, str);
}


// Send command to set "request settings (on startup)" 
//   option on specified device 
// Does not take effect unless "save config" command follows this
function set(i)
{
	var str = "Set request settings for " + json.devices[i-1].type + " on channel " + json.devices[i-1].letter;
	outlet(4, str);
	str = subnet_prefix + '/' + json.devices[i-1].type + i + '/SetRequestSettings';
	outlet(5, str);
}


// Reassigns specified available device to specified free channel
// Immediately updates display
// Sends corresponding set Port and Instance # commands to device
// Does not remain in effect after device restart unless "save config" command follows this
function reassign(from, to)
{
	var str = "Reassigning " + json.devices[from-1].type + " on channel " 
		+ json.devices[from-1].letter + " to " + json.devices[to-1].letter;
	outlet(4, str);
	
	str = subnet_prefix + '/' + json.devices[from-1].type + from + '/SetChannel';
	outlet(5, str, to);
	
	json.devices[from-1].type = "";
	json.devices[from-1].free = 1;
	json.devices[from-1].awake = 0;
	
	update_awake();
	update_receive_gates();
}


// Removes device by instructing it to switch to AP mode
// Does not remain in effect after device restart unless "save config" command follows this
function remove(i)
{
	var str = 'Removed ' + json.devices[i-1].type + ' on channel ' + json.devices[i-1].letter;
	outlet(4, str);
	str = subnet_prefix + '/' + json.devices[i-1].type + i + '/wifiSetup/AP';
	outlet(5, str);
	
	json.devices[i-1].free = 1;
	json.devices[i-1].type = "";
	json.devices[i-1].awake = 0;
	
	outlet(1, '/led', i, 0);
	
	update_awake();
}


// Intercepts battery value messages devices send
// Automatically marks the device as awake and updates display
// if the message came from an accounted for device
function transmitting(i)
{
	if (json.devices[i-1].free == 0) {
		json.devices[i-1].awake = 1;
		outlet(i, '/text', 'textcolor', 0., 0., 0., 1.);
		outlet(1, '/led', i+1, 1);
	}
}

// Takes boolean of whether "Reassign From .. To .." is selected
// Dropdown of free channels is only populated when reassign is true
function set_reassign(e)
{
	json.reassign = e;
}


// Updates the udpreceive objects to check for the ports
// on the subnet
function update_udp()
{
	var subnet = global_port + 10*family_num;
	outlet(6, '/subnet', 'port', subnet);
	for (i = 0; i < 8; i++) {
		outlet(6, '/devices', i+1, 'port', subnet+i+1);
	}
}

function update_routing()
{
	outlet(7, 'set', 1, subnet_prefix);
}


function update_receive_gates()
{
	for (i = 0; i < 8; i++) {
		if (json.devices[i].awake == 1) 
			outlet(8, i+1, 1);
		else
			outlet(8, i+1, 0);
	}	
}
	