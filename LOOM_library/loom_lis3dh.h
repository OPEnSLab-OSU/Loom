// ================================================================ 
// ===                        LIBRARIES                         === 
// ================================================================

// ================================================================ 
// ===                       DEFINITIONS                        === 
// ================================================================
#define i2c_addr_lis3dh 0x44	//0x44, 0x45


// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================
struct config_lis3dh_t {

};

struct state_lis3dh_t {
	Adafruit_SHT31 inst_lis3dh;
	// float temp;
	// float humid;
};

// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================
// struct config_lis3dh_t config_lis3dh;
struct state_lis3dh_t state_lis3dh;


// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
bool setup_lis3dh();
void package_lis3dh(OSCBundle *bndl, char packet_header_string[], uint8_t port);
void package_lis3dh(OSCBundle *bndl, char packet_header_string[]);
void measure_lis3dh();


// ================================================================ 
// ===                          SETUP                           === 
// ================================================================
//
// Runs any SHT31D setup
//
// @return  Whether or not sensor initialization was successful
//
bool setup_lis3dh() {
	// bool is_setup;

	// if (state_lis3dh.inst_lis3dh.begin(i2c_addr_lis3dh)) {
	// 	is_setup = true;
	// 	LOOM_DEBUG_Println("Initialized lis3dh (temp/humid)");
	// } else {
	// 	is_setup = false;
	// 	LOOM_DEBUG_Println("Failed to initialize lis3dh (temp/humid");
	// }
	
	// return is_setup;
}


// ================================================================ 
// ===                        FUNCTIONS                         === 
// ================================================================


// --- PACKAGE SHT31D --- (Multiplexer Version)
// 
// Adds last read SHT31D sensor readings to provided OSC bundle
//
// @param bndl                  The OSC bundle to be added to
// @param packet_header_string  The device-identifying string to prepend to OSC messages
// @param port                  Which port of the multiplexer the device is plugged into
//
void package_lis3dh(OSCBundle *bndl, char packet_header_string[], uint8_t port)
{
// 	char address_string[255];
// 	sprintf(address_string, "%s%s%d%s", packet_header_string, "/port", port, "/lis3dh/data");
	
// 	OSCMessage msg = OSCMessage(address_string);
// 	msg.add("temp").add(state_lis3dh.temp);
// 	msg.add("humid").add(state_lis3dh.humid);
	
// 	bndl->add(msg);
}

void package_lis3dh(OSCBundle *bndl, char packet_header_string[])
{
	// char address_string[255];

	// sprintf(addressString, "%s%s", packet_header_string, "/lis3dh_temp");
	// bndl->add(addressString).add(state_lis3dh.temp);
	// sprintf(addressString, "%s%s", packet_header_string, "/lis3dh_humid");
	// bndl->add(addressString ).add(state_lis3dh.humid);
}


// --- MEASURE SHT31D ---
//
// Gets the current sensor readings of the SHT31D and stores into its state struct
// 
void measure_lis3dh() 
{
	// float t = state_lis3dh.inst_lis3dh.readTemperature();
	// float h = state_lis3dh.inst_lis3dh.readHumidity();

	// if ((!isnan(t)) && (!isnan(h))) {
	// 	state_lis3dh.temp = t;
	// 	state_lis3dh.humid = h;
	// 	LOOM_DEBUG_Println2("Temp: ",     state_lis3dh.temp);
	// 	LOOM_DEBUG_Println2("Humidity: ", state_lis3dh.humid);
	// } else {
	// 	LOOM_DEBUG_Println("Failed to read temperature or humidity");
	// }
}






