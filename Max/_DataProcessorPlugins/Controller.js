
inlets  = 1;
// Inlets
//  0: Commands (match some function name)
outlets = 7;
// Outlets 
//  0: info messages
//  1: error messages
//  2: OSC device port  
//  3: OSC subnet port
//  4: OSC global port
//  5: Outgoing hosts and ports, incoming ports and headers
//  6: Update interface elements


// --- Device Identification ---
var family      = 'LOOM';
var family_num  = 0;
var device_type = 'Template';
var inst_num    = 0;

// --- Communication Info ---
var device_port = 9400;
var subnet_port = 9400;
var global_port = 9400;
var ip_addr     = '192.168.1.255';
var device_targeting = 1; // 1:Device, 2:Subnet, 3:Global 

// --- Variables ---
var i_port    = 0;
var neo_num   = 0;
var red_val   = 0;
var green_val = 0;
var blue_val  = 0;


// --- Generic variables ---
var act_port = 0;
var act_port2 = 0;
var act_num  = 0;

var val0 = 0;
var val1 = 0;
var val2 = 0;
var val3 = 0;
var val4 = 0;

var act_port2_in_use = false; 




// Run the given devices most common command
// The one the executes when 'Send' is presses
function default_cmd()
{
	switch(device_type) {
		case 'Ishield': neopixel_cmd(); break;
		case 'Servo'  : servo_cmd();    break;
		case 'Relay'  : relay_cmd();    break;
		case 'Stepper': stepper_cmd();  break;
 	}
}

// Just call the function to run default commands
function bang()
{
	default_cmd();
}




// ============================================================================
// ===                           COMMON FUNCTIONS                           === 
// ============================================================================


// Check if value is in range
function verify(i, l, h) 
{ 
	return ((i >= l) && (i <= h)); 
}	

// Build the OSC message prefix according provided setting
function target_prefix(i)
{
	switch(i) {
		case 1:  return '/' + family + family_num + '/' + device_type + inst_num;
		case 2:  return '/' + family + family_num;
		case 3:  return '/' + family;
		default: return '';
	}
}

// Set Device Targetting
function targeting(i) { 
	if (verify(i, 1, 3)) {
		device_targeting = i; 
		//outlet(0, 'device targeting', i);
		update_ui();
	} else { 
		outlet(1, i + ' is an invalid targeting option');
	}
}

// Auxiliary generic function to handle sending of OSC messagse
function send(str)
{
	switch(device_targeting) {
		case 1: outlet(0, '(Device) ', str); break;
		case 2: outlet(0, '(Subnet) ', str); break;
		case 3: outlet(0, '(Global) ', str); break;
		default: return;
	}
	outlet(device_targeting+1, str);
}

// Auxiliary generic function to build an OSC message from suffix and args
function build_msg(suffix, args)
{	
	str = target_prefix(device_targeting) + suffix;
	for (i = 0; i < args.length; i++) {
			str += ' ' + args[i];
	}
	return str;
}


// Set device type
function new_device_type(s)
{
	// Only update if it a new device type
	if (device_type != s) {
		device_type = s;

		act_port = act_num =  0;
		val0 = val1 = val2 = val3 = val4 = 0;
	
		update_ui();
		request_ip();	
	}
}

// Set family
function new_family(f)
{
	family = f;
	update_ui();
	request_ip();
}

// Set family number
function new_family_num(i)
{
	if (verify(i, 0, 12)) {
		family_num = i; 
		subnet_port = global_port + 10*family_num;
		device_port = subnet_port + inst_num;
		update_ui();
		request_ip();
	} else {
		outlet(1, i + ' is an invalid family number');
	}
}

// New channel was selected
// Update interface and request new device info
function new_channel(i)
{
	if (verify(i, 0, 8)) {
		inst_num = i;
		device_port = subnet_port + inst_num;
		outlet(5, 'device_port', device_port);
		update_ui();
		request_ip();
	} 
	else outlet(1, i + ' is an invalid Channel');
}

// Set instance number of device to communicate with
// (Legacy, 'new_channel' generally replaces this)
function inst(i)
{ 
	if (verify(i, 0, 1000)) {
		inst_num = i; 
		device_port = subnet_port + inst_num;
		update_ui();
	} else {
		outlet(1, i + ' is an invalid instance number');
	}
}

// Set UDP port
function new_udp_port(i)
{ 
	if (verify(i, 1000, 10000)) {
		device_port = i; 
		outlet(5, 'device_port', i);
	} else {
		outlet(1, i + ' is an invalid UDP port');
	}
	update_ui();
}

// Set IP address 
// (Legacy, 'request_ip' + 'update_ip' generally replace this)
function new_ip(ip) 
{ 
	if (/^(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$/.test(ip)) {  
		ip_addr = ip;
		outlet(5, 'device_host', ip);
	} else {
		outlet(1, ip + 'is an invalid IP address');
	}
	update_ui();
}

// Update IP
function update_ip(device_id,a,b,c,d)
{
	regex = new RegExp(target_prefix(1));
	if (regex.test(device_id)) { // If from device communicating with
		ip_addr = (a+'.'+b+'.'+c+'.'+d);
	}	
	update_ui();
	
	// should set device_host here
}

// Request Device's IP address
function request_ip()
{
	str = target_prefix(1) + '/requestIP';
	//outlet(0, 'Requesting device IP of ' + target_prefix(1));
	outlet(3, str);
}


// Update User Interface Elements
function update_ui()
{
	outlet(6, 'device_type', device_type);
	outlet(6, 'inst',        inst_num);	
	outlet(6, 'ip',          ip_addr);
	outlet(6, 'device_port', device_port);
	outlet(6, 'subnet_port', subnet_port);
	outlet(6, 'global_port', global_port);
	outlet(6, 'addr_preview',  target_prefix(device_targeting));
	outlet(6, 'device_prefix', target_prefix(1));

	
	// Update Max UDP send and receive objects
	outlet(5, 'device_port', device_port);
	outlet(5, 'subnet_port', subnet_port);
	outlet(5, 'global_port', global_port);
	outlet(5, 'device_prefix', target_prefix(1));
	outlet(5, 'subnet_prefix', target_prefix(2));
	outlet(5, 'global_prefix', target_prefix(3));
}


// Print info
function dump()
{
	outlet(0, '***************************');
	outlet(0, device_type + ' Info');
	outlet(0, '***************************');
	outlet(0, 'Family Name: '     + family);
	outlet(0, 'Device Type: '     + device_type);
	outlet(0, 'Instance Number: ' + inst_num);
	outlet(0, 'Device Port: '     + device_port);
	outlet(0, 'Subnet Port: '     + subnet_port);
	outlet(0, 'Global Port: '     + global_port);
	outlet(0, 'IP Address: '      + ip_addr);
	outlet(0, 'Device Targeting: ' + device_targeting);
	outlet(0, '');

	outlet(0, 'Actuator Port: ' + act_port);
	outlet(0, 'Actuator Num: '  + act_num);
	outlet(0, 'Value 0: ' + val0);
	outlet(0, 'Value 1: ' + val1);
	outlet(0, 'Value 2: ' + val2);
	outlet(0, 'Value 3: ' + val3);
	outlet(0, 'Value 4: ' + val4);
	outlet(0, '****************************');
}




// ============================================================================
// ===                 DEVICE VARIALBE UPDATING FUNCTIONS                   === 
// ============================================================================


// Set actuator port 
function set_act_port(i) 
{ 
	if (verify(i, 0, 12)) act_port = i; 
	else outlet(1, i + ' is an invalid port number');
}

function set_act_port2(i) 		
{ 		
	if (verify(i, 0, 12)) {
		act_port2 = i;
		act_port2_in_use = true; 		
	}
	else outlet(1, i + ' is an invalid port number');		
}

// Set actuator number in chain 
function set_act_num(i) 
{ 
	if (verify(i, 0, 64)) act_num = i; 
}

// Set val# variables
function set_val0(i) { val0 = i; }
function set_val1(i) { val1 = i; }
function set_val2(i) { val2 = i; }
function set_val3(i) { val3 = i; }
function set_val4(i) { val4 = i; }




// ============================================================================
// ===                      DEVICE SPECIFIC FUNCTIONS                       === 
// ============================================================================


// Output Neopixel command 
function neopixel_cmd()
{	
	send( build_msg( '/Neopixel', [act_port, act_num, val0, val1, val2] ) );
}

// Output command to set relay
function relay_cmd()
{
	send( build_msg( '/Relay/State', [act_port, val0] ) );
}

// Output command to set servo
function servo_cmd()
{
	send( build_msg( '/Servo/Set', [act_port, val0] ) );
	if (act_port2_in_use) {		
 		send( build_msg( '/Servo/Set', [act_port2, val1] ) );		
 	}
}

// Output command to set stepper
function stepper_cmd()
{
	send( build_msg( '/Stepper/Set', [act_port, val0, val1, val2] ) );
}

// Refresh the list of sensors on multiplexer
function refresh_mux()
{
	send( build_msg( '/GetSensors', [] ) );	
}

// SD control function
// Takes on/off value directly
function enable_SD(s)
{
	send( build_msg( '/EnableSD', [s] ) );	
}

// Change the sample rate / loop sleep period
function set_sample_rate(t)
{
	send( build_msg( '/SetSampleRate', [t] ) );
}

// Save config
function save_config()
{
	send( build_msg( '/SaveConfig', [] ) );	
}

function cal_mpu()
{
	send( build_msg( '/cal_MPU6050', [] ) );	
}

function sheet_id(s)
{
	send( build_msg( '/SetSpreadSheetID', [s] ) );	
}

function tab_id(s)
{
	send( build_msg( '/SetTabID', [s] ) );	
}

function upload_interval(d)
{
	send( build_msg( '/SetPushMinDelay', [d] ) );	
}
