// ================================================================
// ===                        LIBRARIES                         ===
// ================================================================


// ================================================================ 
// ===                       DEFINITIONS                        === 
// ================================================================
#define HEATPIN 12

// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================

struct state_sapflow_t {
	float temp0, temp1, diff;
};

// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================

unsigned long lastUpdate;
unsigned long currentTime;
bool sapflow_relay_on = false;

struct state_sapflow_t state_sapflow;


// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================

void package_sapflow(OSCBundle *bndl, char packet_header_string[]);
void measure_sapflow();
double voltTotemp(double vout);
void heat(uint16_t pulse);


// ================================================================ 
// ===                          SETUP                           === 
// ================================================================
//
// Runs any startup for sapflowmeter that should occur on device startup
// 
// @return  Whether or not sensor initialization was successful
//
void setup_sapflow() 
{
	pinMode(HEATPIN,OUTPUT);
	lastUpdate = millis();
}


// ================================================================ 
// ===                        FUNCTIONS                         === 
// ================================================================

// --- SAP FLOW METER
//Convert voltage output to temperature value
double voltTotemp( double vout )
{ 
	double vcc = 3300; //3.3V
	double Ro = 100000.0;
	double To = 298.15;
	double B = 4700.0;

	double Rbias = 100000.0;
	double long R = ((vcc / vout) - 1) * Ro;

	double long T = B / ( log( R * exp( B / To) ) - (log(Ro) ) ) - 273.15 ;
	return T;
}

void heat(uint16_t pulse)
{ //heatpulse = 0: TDM, heatpulse > 1: HRM
//  int heatpulse = 1000; //http://www.open-sensing.org/sapflowmeter-blog/2018/6/4/sap-flux-heat-calculations

	currentTime = millis();
	if (currentTime - lastUpdate > pulse) {
		digitalWrite(HEATPIN, sapflow_relay_on ? HIGH : LOW);
		sapflow_relay_on = !sapflow_relay_on;
		lastUpdate = currentTime;  
	}

}
	
// --- PACKAGE <MODULE> ---
//
// Adds OSC Message of most recent sensor readings to a provided OSC bundle
//
// @param bndl                  The OSC bundle to be added to
// @param packet_header_string  The device-identifying string to prepend to OSC messages
//   if I2C multiplexer sensor, then also
// [@param port                  Which port of the multiplexer the device is plugged into]
//
void package_sapflow(OSCBundle *bndl, char packet_header_string[]) 
{
	char address_string[255];
	OSCMessage msg;

	sprintf(address_string, "%s%s\0", packet_header_string, "/temp0");
	msg.add(state_sapflow.temp0);
	msg.setAddress(address_string);
	bndl->add(msg);
	msg.empty();

	sprintf(address_string, "%s%s\0", packet_header_string, "/temp1");
	msg.add(state_sapflow.temp1);
	msg.setAddress(address_string);
	bndl->add(msg);
	msg.empty();
		
	sprintf(address_string, "%s%s\0", packet_header_string, "/temp_diff");
	msg.add(state_sapflow.diff);
	msg.setAddress(address_string);
	bndl->add(msg);
	msg.empty();
}


// --- MEASURE ANALOG ---
//
// Measure analog data and update analog state to most recent readings. 
//
void measure_sapflow() 
{
	double temp;

	temp = read_analog(0);  //analog Read gives values from 0-1023 based on 0-3.3V
	temp = map(temp, 0, 4095, 0, 3300);//map these to mV value
	state_sapflow.temp1 = voltTotemp(temp);
	state_sapflow.temp1 = temp;

	temp = read_analog(1);  //analog Read gives values from 0-1023 based on 0-3.3V
	temp = map(temp, 0, 4095, 0, 3300);//map these to mV value
	state_sapflow.temp1 = voltTotemp(temp);
	state_sapflow.temp1 = temp;

	state_sapflow.diff = state_sapflow.temp0 - state_sapflow.temp1;
	if (state_sapflow.diff < 0) {
		state_sapflow.diff *= -1;
	}
}
