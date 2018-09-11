// ---------- LOOP -----------------
//  #if is_sapflow == 1
//    run_sapflowmeter(&send_bndl);
//  #endif
// ---------- ----------------------  
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
  float temp0, temp1, temp_diff;
};

// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================

unsigned long lastUpdate;//heat duration
unsigned long currentTime;//heat duration

unsigned long lastPulseUpdate;//heat pulse
unsigned long currentPulseTime;//heat pulse

unsigned long startMillis;//send data
unsigned long currentMillis;//send data

bool sapflow_relay_on = false;
bool pulseflag = false;

struct state_sapflow_t state_sapflow;
uint8_t testcounter;

char FileName[] = "LOG0806.CSV";
// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
#if is_node == 1
void package_sapflow(OSCBundle *bndl, char packet_header_string[]);
void measure_sapflow();
double voltTotemp(double vout);
void heat();
void senddata();
#endif
void run_sapflowmeter(OSCBundle *bndl);

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
  #if is_node == 1
    setup_sht31d();
//    sd_delete_file(FileName);
    pinMode(HEATPIN,OUTPUT);
    lastUpdate = millis();
    startMillis = millis();
    lastPulseUpdate = millis();
    
    testcounter = 0;
  #endif

}


// ================================================================ 
// ===                        FUNCTIONS                         === 
// ================================================================

#if is_node == 1

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

void senddata()
{
  OSCBundle bndl;
 
  currentMillis = millis();
  if (currentMillis - startMillis >= senddelay) {
    measure_sensors();
    package_data(&bndl);
    #if LOOM_DEBUG == 1
      print_bundle(&bndl);
    #endif
//    send_bundle(&bndl, LORA); //
    lora_send_bundle(&bndl); 

    sd_save_bundle(FileName, &bndl, 0, 3);
    //  read_all_from_file("newlog");
    startMillis = currentMillis;
  }
}

void testsenddata()
{
  OSCBundle bndl;

  if (currentMillis - startMillis >= 6000) {
    if (testcounter <= 2) {
      measure_sensors();
      Serial.print("\n\n< Data log test [");Serial.print(testcounter+1);Serial.print("/3]... >\n");
      package_data(&bndl);
      lora_send_bundle(&bndl);

      #if LOOM_DEBUG == 1
        print_bundle(&bndl);
      #endif
      sd_save_bundle(FileName, &bndl, 0, 3);
      testcounter++;
    }
//    if(testcounter == 3){
//      sd_save_bundle(FileName, &bndl, 0, 3);
//      print_bundle(&bndl);
//      Serial.println("Test Success!");Serial.println("Turn on the Heater, please");
//      testcounter++;
//    }
    
  }
}


// --- HEAT ---
//
// Turn on heater every "heatpulse" seconds
//
void heat()
{
  currentTime = millis();
  
  #if probe_type  == 0      // 0:TDM, 1: HRM
      if ( (currentTime - lastUpdate >= senddelay - heatduration) && (currentTime - lastUpdate < senddelay) ){//turn on 10min before sending data
        digitalWrite(HEATPIN, HIGH); //turn on heater
      }
      else if(currentTime - lastUpdate >= senddelay)
      {
        digitalWrite(HEATPIN, HIGH); //turn on heater
        lastUpdate = currentTime;
        pulseflag = true;
      }
      if(pulseflag){
        if(currentTime - lastUpdate >= 10000){
          digitalWrite(HEATPIN, LOW); //turn off heater
          pulseflag = false;
        } 
      }   
  #endif //of probe_type == 0
  
  #if probe_type == 1      // 0:TDM, 1: HRM
    currentPulseTime = millis();
    
    if ( (currentTime - lastUpdate >= senddelay - heatduration) && (currentTime - lastUpdate < senddelay) ){//turn on pulse 1 min before sending data
      if (currentPulseTime - lastPulseUpdate > heatpulse) {//turn on/off every 2.5 sec
        digitalWrite(HEATPIN, sapflow_relay_on ? HIGH : LOW);
        sapflow_relay_on = !sapflow_relay_on;
        lastPulseUpdate = currentPulseTime;  
      }
    }
    else if(currentTime - lastUpdate >= senddelay){ 
      if (currentPulseTime - lastPulseUpdate > heatpulse) {//turn on/off every 2.5 sec
        digitalWrite(HEATPIN, sapflow_relay_on ? HIGH : LOW);
        sapflow_relay_on = !sapflow_relay_on;
        lastPulseUpdate = currentPulseTime;  
      }
      lastUpdate = currentTime;
      pulseflag = true;  
    }
    if(pulseflag){
      if (currentPulseTime - lastPulseUpdate > heatpulse) {//turn on/off every 2.5 sec
        digitalWrite(HEATPIN, sapflow_relay_on ? HIGH : LOW);
        sapflow_relay_on = !sapflow_relay_on;
        lastPulseUpdate = currentPulseTime;  
      }
      if(currentTime - lastUpdate >= 10000){ //Turn off pulse 10 sec after sending data
        digitalWrite(HEATPIN, LOW); //turn off heater
        pulseflag = false;
      } 
    }
  #endif //of probe_type == 1
  

}

void package_sapflow(OSCBundle *bndl, char packet_header_string[]) 
{
  char address_string[255];
  
  sprintf(addressString, "%s%s", packet_header_string, "/temp0");
  bndl->add(addressString).add(state_sapflow.temp0);
  sprintf(addressString, "%s%s", packet_header_string, "/temp1");
  bndl->add(addressString ).add(state_sapflow.temp1);
  sprintf(addressString, "%s%s", packet_header_string, "/temp_diff");
  bndl->add(addressString).add(state_sapflow.temp_diff);
}


// --- MEASURE Temperature ---
//
// Measure analog data and update analog state to most recent readings. 
//
void measure_sapflow()  
{
  double temp;

  temp = read_analog(0);  //analog Read gives values from 0-1023(or 4095 at 12bit) based on 0-3.3V
  temp = map(temp, 0, 4095, 0, 3300);//map these to mV value
  state_sapflow.temp0 = voltTotemp(temp);

  temp = read_analog(1);  //analog Read gives values from 0-1023 based on 0-3.3V
  temp = map(temp, 0, 4095, 0, 3300);//map these to mV value
  state_sapflow.temp1 = voltTotemp(temp);

  state_sapflow.temp_diff = state_sapflow.temp0 - state_sapflow.temp1;
  if (state_sapflow.temp_diff < 0) {
    state_sapflow.temp_diff *= -1;
  }
}

#endif // of is_node == 1


void run_sapflowmeter(OSCBundle *bndl)
{
    #if is_hub == 1
    // Receive bundles, takes bundle to be filled and wireless platforms [WIFI, LORA, NRF]
//      receive_bundle(bndl, LORA);
      lora_receive_bundle(bndl);

      if (bundle_empty(bndl)) return;

      print_bundle(bndl);

    #endif // of is_hub

    #if is_node == 1
      testsenddata(); 
      if(testcounter>=2){
        senddata();  
        heat();
      }
    #endif // of is_node
}



