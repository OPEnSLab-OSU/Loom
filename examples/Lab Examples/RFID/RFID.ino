///////////////////////////////////////////////////////////////////////////////

// This example is the code used for the OPEnS RFID-VWC

// It is using the following modules:
// - RFID
// - Neopixel
// - Wifi

// Normal operation is:
// - Collect data
// - Run State Machine

///////////////////////////////////////////////////////////////////////////////


#include <Loom.h>

#define RELAY_SWITCH A0
//#define FERTIGATE_SWITCH A2
#define RELAY_RSSI_THRESHOLD -80
#define RSSI_THRESHOLD -60
#define WET 16
#define DRY 21
#define NEOPIXEL A1
#define NUMPIXELS 1
#define MAX_VALUE 26
#define MIN_VALUE 17


// Include configuration
const char* json_config =
#include "config.h"
;

// Set enabled modules
LoomFactory<
	Enable::Internet::WiFi,
	Enable::Sensors::Enabled,
	Enable::Radios::Disabled,
	Enable::Actuators::Enabled,
	Enable::Max::Disabled
> ModuleFactory{};

LoomManager Loom{ &ModuleFactory };

char color[10];
int sketch_index=-1;
bool _debug_serial = true;
bool results = false;
Loom_RFID* rfid;
//uint8_t fertigate_state = 0;

void setup()
{
	pinMode(RELAY_SWITCH, OUTPUT);
	//pinMode(FERTIGATE_SWITCH, INPUT_PULLUP);

  Loom.begin_serial(true);
	Loom.parse_config(json_config);
	Loom.print_config();
	LPrintln("\n ** Setup Complete ** ");

	rfid = (Loom_RFID*)Loom.find_module(LoomModule::Type::RFID);
}

void loop()
{
	//readFertigateSwitch();
  Loom.measure();
 	sketch_index = rfid->getTagIndex();
	if(sketch_index!=-1){
		Loom_RFID::tag found_tag = rfid->getTag(sketch_index);
		results = rfid->isUpdatedIndex();
		LPrintln("Tag Index Measured: ", sketch_index);
	  //if((results && !fertigate_state) || (results && found_tag.fertigate == 0xFF))
		if(results)
			state_machine(found_tag, sketch_index);
	  else LPrintln("State machine not invoked");

		int moisture = found_tag.moisture, rssi = found_tag.rssi;
		setColor(moisture, rssi);
	}
	else if(rfid->get_error_counter() < 10) setColor(-1, -1);

	// Figure out where all of this code will go?
  // Loom.package();
  // Loom.display_data();
	// Loom.publish();
  // Loom.pause();
}

void setColor(int moisture, int rssi){
  if(moisture >= 0 && rssi >= RSSI_THRESHOLD){
      if(moisture <= WET){
           for(int i = 0; i < NUMPIXELS; i++){
              Loom.Neopixel().set_color(NEOPIXEL, i, 0, 0, 150);
              strcpy(color, "blue"); //Add to Loom data or add to struct field?
           }
      }
      else if(moisture < DRY){
           for(int i = 0; i < NUMPIXELS; i++){
              Loom.Neopixel().set_color(NEOPIXEL, i, 0, 150, 0);
              strcpy(color, "green"); //Add to Loom data or add to struct field?
           }
      }
      else{
          for(int i = 0; i < NUMPIXELS; i++){
              Loom.Neopixel().set_color(NEOPIXEL, i, 150, 0, 0);
              strcpy(color, "red"); //Add to Loom data or add to struct field?
           }
      }
  }
  else{
       for(int i = 0; i < NUMPIXELS; i++){
          Loom.Neopixel().set_color(NEOPIXEL, i, 0, 0, 0);
       }
  }
}

void turnOnRelay(){
	digitalWrite(RELAY_SWITCH, HIGH);
}

void turnOffRelay(){
	digitalWrite(RELAY_SWITCH, LOW);
}

//void readFertigateSwitch(){
//	if(digitalRead(FERTIGATE_SWITCH) == HIGH){
//		fertigate_state = 1;
//	}
//	else{
//		fertigate_state = 0;
//	}
//}


void state_machine(Loom_RFID::tag found_tag, int i){

  switch(found_tag.state){
    case Loom_RFID::State::COMPARE_THRESHOLD:

           if(_debug_serial){
              LPrint("EPC Tag: ");
              LPrint_Hex(found_tag.EPCHeaderName);
              LPrintln(" COMPARE_THRESHOLD state");
           }

           if(found_tag.threshold == MAX_VALUE){
              if(_debug_serial){
                 LPrintln("Transitioning to: DRY_CYCLE");
                 LPrintln();
              }
              found_tag.state = Loom_RFID::State::DRY_CYCLE;
              rfid->setTag(i, found_tag);
           }
           else if(found_tag.threshold == MIN_VALUE){
              if(_debug_serial){
                 LPrintln("Transitioning to: WET_CYCLE");
                 LPrintln();
              }
              found_tag.state = Loom_RFID::State::WET_CYCLE;
              rfid->setTag(i, found_tag);
           }

           break;

         case Loom_RFID::State::WET_CYCLE:

          if(_debug_serial){
             LPrint("EPC Tag: ");
             LPrint_Hex(found_tag.EPCHeaderName);
             LPrint(" Current RSSI: ");
             LPrint(found_tag.rssi);
             LPrint(" WET_CYCLE state ");
             LPrint("Current moisture: ");
             LPrint(found_tag.moisture);
             LPrint(" Threshold moisture: ");
             LPrintln(found_tag.threshold);
             LPrintln();
          }

           if(found_tag.moisture > found_tag.threshold){
           	 if(found_tag.rssi > RELAY_RSSI_THRESHOLD){
              	turnOnRelay();
              	found_tag.state = Loom_RFID::State::COMPARE_THRESHOLD;
                rfid->setTag(i, found_tag);
           	 }
           	 else
           	 	turnOffRelay();
           }
           else if(found_tag.moisture <= found_tag.threshold){
              found_tag.threshold = MAX_VALUE;
              found_tag.state = Loom_RFID::State::COMPARE_THRESHOLD;
              rfid->setTag(i, found_tag);
              turnOffRelay();
           }

           break;

         case Loom_RFID::State::DRY_CYCLE:

            if(_debug_serial){
               LPrint("EPC Tag: ");
               LPrint(found_tag.EPCHeaderName, HEX);
               LPrint(" Current RSSI: ");
               LPrint(found_tag.rssi);
               LPrint(" DRY_CYCLE state ");
               LPrint("Current moisture: ");
               LPrint(found_tag.moisture);
               LPrint(" Threshold moisture: ");
               LPrintln(found_tag.threshold);
               LPrintln();
            }

            if(found_tag.moisture > found_tag.threshold){
            	  if(found_tag.rssi > RELAY_RSSI_THRESHOLD){
            	  		found_tag.threshold = MIN_VALUE;
                    rfid->setTag(i, found_tag);
                		turnOnRelay();
                		found_tag.state = Loom_RFID::State::COMPARE_THRESHOLD;
                    rfid->setTag(i, found_tag);

            	  }

            }
            else if(found_tag.moisture <= found_tag.threshold){
              found_tag.state = Loom_RFID::State::COMPARE_THRESHOLD;
              rfid->setTag(i, found_tag);
              turnOffRelay();
            }
  }
}
