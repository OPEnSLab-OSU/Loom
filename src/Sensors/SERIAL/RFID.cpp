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
                    MeasureType t,
                    int tags,
                    int max,
                    int min,
                    int RX,
                    int TX,
                    int num_samples
                    )
: LoomSerialSensor(manager, "RFID", Type::RFID, num_samples)
  , type(t)
  , num_tags(tags)
  , MAX_VAL(max)
  , MIN_VAL(min)
  , rfid_serial(&sercom1, RX, TX, SERCOM_RX_PAD_0, UART_TX_PAD_2)
{
  tag_counter = 0;
  error_counter=0;
  index=-1;
  updated=false;
  rfid_tags = new tag[num_tags];
  LPrintln("Begining RFID Serial at 115200bps");
  rfid_serial.begin(115200);
  while(!rfid_serial);

  int count = 0;

  while(!setupNano() && count<5){
    LPrintln("Module is not responding, check the wiring");
    count++;
    delay(1000);
  }
  if(count == 5){
    LPrintln("Failed to setup Nano after five attemps");
    return;
  }
  else{
    LPrintln("Setting RFID Region and read power");
    nano.setRegion(REGION_NORTHAMERICA);
    nano.setReadPower(2600);
    LPrintln("RFID Reader is staring to read");
    nano.startReading();
  }
}

///////////////////////////////////////////////////////////////////////////////

Loom_RFID::Loom_RFID(LoomManager* manager, JsonArrayConst p)
: Loom_RFID(manager, (MeasureType)(int)
p[0], (int)p[1], (int)p[2], (int)p[3], (int)p[4], (int)p[5], (int)p[6])
{}

///////////////////////////////////////////////////////////////////////////////

Loom_RFID::~Loom_RFID(){
  LPrintln("RFID Destructor Called");
  delete[] rfid_tags;
}

///////////////////////////////////////////////////////////////////////////////

bool Loom_RFID::setupNano(){
  delay(200);
  nano.enableDebugging();
  nano.begin(rfid_serial);
  //Test to see if we are already connected to a module
	//This would be the case if the Arduino has been reprogrammed and the module has stayed powered
  while(!rfid_serial);
  //About 200ms from power on the module will send its firmware version at 115200. We need to ignore this.
  while(rfid_serial.available()) rfid_serial.read();
  delay(100);
  LPrintln("Getting RFID Tag Reader Version...");
  nano.getVersion();

  if (nano.msg[0] == ERROR_WRONG_OPCODE_RESPONSE)
	{
	 	//This happens if the baud rate is correct but the module is doing a ccontinuous read
	 	nano.stopReading();
	 	LPrintln("Module continuously reading. Asking it to stop...");
	 	delay(1500);
	}
  else{
    //The module did not respond so assume it's just been powered on and communicating at 115200bps
    LPrintln("Setting Baud...");
    nano.setBaud(115200); //Tell the module to go to the chosen baud rate. Ignore the response msg
  }

  //Test the connection
  LPrintln("Getting Version...");
  nano.getVersion();
  delay(200);
  nano.getVersion();

  if (nano.msg[0] != ALL_GOOD){
    LPrintln_Hex(nano.msg[0]);
    return false; //Something is not right
  }

  LPrintln_Hex(nano.msg[0]);
  LPrintln("Setting Tag Protocol...");
  //The M6E has these settings no matter what
  nano.setTagProtocol(); //Set protocol to GEN2

  LPrintln("Setting Antenna Port...");
  nano.setAntennaPort(); //Set TX/RX antenna ports to 1
  nano.disableDebugging();

  return true;
}

///////////////////////////////////////////////////////////////////////////////

void Loom_RFID::addNewTag(int i, byte EPCHeader, byte EPCFertigate){
   i++;
   tag newTag;
   newTag.EPCHeaderName = EPCHeader;
   newTag.fertigate     = EPCFertigate;
   newTag.threshold     = MIN_VAL;
   newTag.state         = State::COMPARE_THRESHOLD;

   rfid_tags[i] = newTag;
}

///////////////////////////////////////////////////////////////////////////////

void Loom_RFID::updateData(int i, int rssi, long freq, long moisture, byte tagEPCBytes, byte EPCFertigate){
   rfid_tags[i].fertigate   = EPCFertigate;
   rfid_tags[i].rssi        = rssi;
   rfid_tags[i].moisture    = moisture;
   rfid_tags[i].freq        = freq;
   rfid_tags[i].tagEPCBytes = tagEPCBytes;
}


///////////////////////////////////////////////////////////////////////////////

void Loom_RFID::measure(){
  switch (type)
  {
    case MeasureType::Moisture:
      LPrintln("Moisture tags will be measured");
      moistureTagMeasurement();
      break;
    case MeasureType::Unknown:
      LPrintln("Unknown measure type");
      break;
    default:
      LPrintln("No known declared measure type");
      break;
  }
}

///////////////////////////////////////////////////////////////////////////////

void Loom_RFID::moistureTagMeasurement(){
  int rssi, timeout_counter = 0;
  long freq, moisture;
  byte tagEPCBytes, EPCHeader, EPCFertigate;
  if(nano.check() == true){
    byte responseType = nano.parseResponse();

    if(responseType == RESPONSE_IS_KEEPALIVE)
      LPrintln("Nano is Scanning");
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
            updateData(i, rssi, freq, moisture, tagEPCBytes, EPCFertigate);
          }
          else if(i == tag_counter && tag_counter < num_tags){
            addNewTag(i, EPCHeader, EPCFertigate);
            tag_counter++;
          }

        }
        error_counter=0;
      }
      else error_counter++;
    }
    else if(responseType == ERROR_CORRUPT_RESPONSE)
      LPrintln("Bad CRC");
    else
      LPrintln("Nano is Scanning...");
  }
}

///////////////////////////////////////////////////////////////////////////////

void Loom_RFID::package(JsonObject json){
  JsonObject data = get_module_data_object(json, module_name);
  char EPCHeader_Hex[3];
  JsonArray array = data.createNestedArray("Tags");

  for(int i=0; i<tag_counter;i++){
     JsonObject tag = array.createNestedObject();
     itoa(rfid_tags[i].EPCHeaderName, EPCHeader_Hex, 16);
     tag["RSSI"] = rfid_tags[i].rssi;
     tag["Frequency"] = rfid_tags[i].freq;
     tag["Moisture"] = rfid_tags[i].moisture;
     tag["EPC"] = EPCHeader_Hex;
  }
}

///////////////////////////////////////////////////////////////////////////////

void Loom_RFID::print_measurements() const{
  print_module_label();
  char EPCHeader_Hex[3];
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

int Loom_RFID::getTagIndex(){
  return index;
}

///////////////////////////////////////////////////////////////////////////////

boolean Loom_RFID::isUpdatedIndex(){
  return updated;
}

///////////////////////////////////////////////////////////////////////////////

Loom_RFID::tag Loom_RFID::getTag(int i){
  if(i >= 0 && i <tag_counter)
    return rfid_tags[i];
  else{
    LPrintln("No found tag at this index");
    tag emptyTag;
    return emptyTag;
  }
}

///////////////////////////////////////////////////////////////////////////////

void Loom_RFID::setTag(int i, tag t){
  if(i >= 0 && i <tag_counter)
    rfid_tags[i] = t;
  else{
    LPrintln("No found tag at this index");
    return;
  }
}

///////////////////////////////////////////////////////////////////////////////
