///////////////////////////////////////////////////////////////////////////////
///
/// @file		RFID.cpp
/// @brief		File for RFID Reader implementation.
/// @author		Adam Kerr
/// @date		2020
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#include "RFID.h"

///////////////////////////////////////////////////////////////////////////////

Loom_RFID::Loom_RFID(LoomManager* manager,
                    int tags,
                    int max,
                    int min,
                    MeasureType t,
                    int num_samples
                    )
: LoomSerialSensor(manager, "RFID", Type::RFID, num_samples)
  , type(t)
  , num_tags(tags)
  , MAX_VAL(max)
  , MIN_VAL(min)
{
  tag_counter = 0;
  error_counter = 0;
  index=-1;
  updated=false;
  rfid_tags = new tag[num_tags];
}

///////////////////////////////////////////////////////////////////////////////

Loom_RFID::Loom_RFID(LoomManager* manager, JsonArrayConst p)
: Loom_RFID(manager, (int)p[0], (int)p[1], (int)p[2], (MeasureType)(int)p[3], (int)p[4])
{}

///////////////////////////////////////////////////////////////////////////////

Loom_RFID::~Loom_RFID(){
  print_module_label();
  LPrintln("RFID Destructor Called");
  delete[] rfid_tags;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_RFID::setup_nano(){
  if(is_setup) return;
  int count = 0;

  while(!setup_nano_impl() && count<5){
    print_module_label();
    LPrintln("Module is not responding, check the wiring");
    count++;
    delay(1000);
  }
  if(count == 5){
    print_module_label();
    LPrintln("Failed to setup Nano after five attempts");
    return;
  }
  else{
    is_setup = true;
    print_module_label();
    LPrintln("Setting RFID Region and read power");
    nano.setRegion(REGION_NORTHAMERICA);
    nano.setReadPower(2600);
    print_module_label();
    LPrintln("RFID Reader is starting to read");
    nano.startReading();
  }
}


///////////////////////////////////////////////////////////////////////////////

bool Loom_RFID::setup_nano_impl(){
  print_module_label();
  LPrintln("Beginning Nano setup");
  delay(200);
  nano.enableDebugging();
  nano.begin(*sensor_serial);

  //Test to see if we are already connected to a module
	//This would be the case if the Arduino has been reprogrammed and the module has stayed powered
  while(sensor_serial == nullptr);

  //About 200ms from power on the module will send its firmware version at 115200. We need to ignore this.
  while(sensor_serial->available()) sensor_serial->read();
  delay(100);
  print_module_label();
  LPrintln("Getting RFID Tag Reader Version...");
  nano.getVersion();

  if (nano.msg[0] == ERROR_WRONG_OPCODE_RESPONSE)
	{
	 	//This happens if the baud rate is correct but the module is doing a ccontinuous read
	 	nano.stopReading();
    print_module_label();
	 	LPrintln("Module continuously reading. Asking it to stop...");
	 	delay(1500);
	}
  else{
    //The module did not respond so assume it's just been powered on and communicating at 115200bps
    print_module_label();
    LPrintln("Setting Baud...");
    nano.setBaud(115200); //Tell the module to go to the chosen baud rate. Ignore the response msg
  }

  //Test the connection
  print_module_label();
  LPrintln("Getting Version...");
  nano.getVersion();
  delay(200);
  nano.getVersion();

  if (nano.msg[0] != ALL_GOOD){
    print_module_label();
    LPrintln_Hex(nano.msg[0]);
    return false; //Something is not right
  }

  LPrintln_Hex(nano.msg[0]);
  print_module_label();
  LPrintln("Setting Tag Protocol...");
  //The M6E has these settings no matter what
  nano.setTagProtocol(); //Set protocol to GEN2

  print_module_label();
  LPrintln("Setting Antenna Port...");
  nano.setAntennaPort(); //Set TX/RX antenna ports to 1
  nano.disableDebugging();

  return true;
}

///////////////////////////////////////////////////////////////////////////////

void Loom_RFID::add_new_tag(int i, byte EPCHeader, byte EPCFertigate){
   i++;
   tag newTag;
   newTag.EPCHeaderName = EPCHeader;
   newTag.fertigate     = EPCFertigate;
   newTag.threshold     = MIN_VAL;
   newTag.state         = State::COMPARE_THRESHOLD;

   set_tag(i, newTag);
}

///////////////////////////////////////////////////////////////////////////////

void Loom_RFID::update_data(int i, int rssi, long freq, long moisture, byte tagEPCBytes, byte EPCFertigate){
   rfid_tags[i].fertigate   = EPCFertigate;
   rfid_tags[i].rssi        = rssi;
   rfid_tags[i].moisture    = moisture;
   rfid_tags[i].freq        = freq;
   rfid_tags[i].tagEPCBytes = tagEPCBytes;
}


///////////////////////////////////////////////////////////////////////////////

void Loom_RFID::measure(){
  if(!is_setup) setup_nano();
  switch (type)
  {
    case MeasureType::Moisture:
      print_module_label();
      LPrintln("Moisture tags will be measured");
      moisture_tag_measurement();
      break;
    case MeasureType::Unknown:
      print_module_label();
      LPrintln("Unknown measure type");
      break;
    default:
      print_module_label();
      LPrintln("No known declared measure type");
      break;
  }
}

///////////////////////////////////////////////////////////////////////////////

void Loom_RFID::moisture_tag_measurement(){
  int rssi, timeout_counter = 0;
  long freq, moisture;
  byte tagEPCBytes, EPCHeader, EPCFertigate;
  if(nano.check() == true){
    byte responseType = nano.parseResponse();

    if(responseType == RESPONSE_IS_KEEPALIVE){
      print_module_label();
      LPrintln("Nano is Scanning");
    }
    else if(responseType == RESPONSE_IS_TAGFOUND){
      index=-1;
      updated=false;
      rssi          = nano.getTagRSSINew(); 		//Get the RSSI for this tag read
      freq          = nano.getTagFreqNew(); 		//Get the frequency this tag was detected at
      tagEPCBytes   = nano.getTagEPCBytesNew(); //Get the number of bytes of EPC from response
      EPCHeader     = nano.getEPCHeader();		//Get the EPC header for identification
      EPCFertigate  = nano.getFertigateTag();	//Get the EPC fertigate code
      moisture      = nano.getMoistureData();	//Get the moisture data of the current read

      if(freq < 1000000 && nano.getAntennaeIDNew() == 17 && tagEPCBytes <= 20){
        for(int i=0; i <= tag_counter; i++){
          index=i;
          if(rfid_tags[i].EPCHeaderName == EPCHeader){
            updated=true;
            update_data(i, rssi, freq, moisture, tagEPCBytes, EPCFertigate);
          }
          else if(i == tag_counter && tag_counter < num_tags){
            tag_counter++;
            add_new_tag(i, EPCHeader, EPCFertigate);
          }

        }
        error_counter=0;
      }
      else error_counter++;
    }
    else if(responseType == ERROR_CORRUPT_RESPONSE){
      print_module_label();
      LPrintln("Bad CRC");
    }
    else{
      print_module_label();
      LPrintln("Nano is Scanning...");
    }
  }
}

///////////////////////////////////////////////////////////////////////////////

void Loom_RFID::package(JsonObject json){
  char EPCHeader_Hex[3];
  for(int i=0; i<tag_counter;i++){
     JsonObject data = get_module_data_object(json, "RFID Tag " + i);
     itoa(rfid_tags[i].EPCHeaderName, EPCHeader_Hex, 16);
     data["RSSI"] = rfid_tags[i].rssi;
     data["Frequency"] = rfid_tags[i].freq;
     data["Moisture"] = rfid_tags[i].moisture;
     data["EPC"] = EPCHeader_Hex;
  }
}

///////////////////////////////////////////////////////////////////////////////

void Loom_RFID::print_measurements() const{
  print_module_label();
  char EPCHeader_Hex[3];
  print_module_label();
  LPrintln("Measurements:");
  for(int i=0; i<tag_counter; i++){
    itoa(rfid_tags[i].EPCHeaderName, EPCHeader_Hex, 16);
    LPrintln("\tRFID Tag #", i+1);
    LPrintln("\t\tRSSI       :", rfid_tags[i].rssi);
    LPrintln("\t\tFrequency  :", rfid_tags[i].freq);
    LPrintln("\t\tMoisture   :", rfid_tags[i].moisture);
    LPrintln("\t\tEPC        :", EPCHeader_Hex);
  }
}

///////////////////////////////////////////////////////////////////////////////

Loom_RFID::tag Loom_RFID::get_tag(int i){
  if(i >= 0 && i <tag_counter)
    return rfid_tags[i];
  else{
    print_module_label();
    LPrintln("No found tag at this index");
    tag emptyTag;
    return emptyTag;
  }
}

///////////////////////////////////////////////////////////////////////////////

void Loom_RFID::set_tag(int i, tag t){
  if(i >= 0 && i <tag_counter)
    rfid_tags[i] = t;
  else{
    print_module_label();
    LPrintln("No found tag at this index");
    return;
  }
}

///////////////////////////////////////////////////////////////////////////////
