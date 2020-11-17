///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_BatchSD.cpp
/// @brief		File for Loom_BatchSD implementation.
/// @author		Adam Kerr
/// @date		2020
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#include "BatchSD.h"
#include "Manager.h"

#include <SPI.h>

///////////////////////////////////////////////////////////////////////////////

Loom_BatchSD::Loom_BatchSD(
    LoomManager* manager,
    const bool			enable_rate_filter,
    const uint16_t		min_filter_delay,
    const byte			chip_select

    )
    : LoomLogPlat(manager, "BatchSD", Type::BATCHSD, enable_rate_filter, min_filter_delay )
    , chip_select(chip_select)
    , doc(2048)
{
  LMark;
  digitalWrite(8, HIGH); // if using LoRa, need to temporarily prevent it from using SPI
  LMark;

  bool sd_found = sd.begin(chip_select, SD_SCK_MHZ(50));
  LMark;
  batch_counter=0;
  LMark;
  packet_counter=0;
  LMark;
  drop_count = 0;
  LMark;
  if(sd_found) {
    LMark;
    // Setup Directory for Batch Files
    sd.mkdir("Batches");
    LMark;
  }

  print_module_label();
  LMark;
  LPrintln("Initialize ", (sd_found) ? "sucessful" : "failed");
  LMark;

}

///////////////////////////////////////////////////////////////////////////////
Loom_BatchSD::Loom_BatchSD(LoomManager* manager, JsonArrayConst p)
  : Loom_BatchSD(manager,EXPAND_ARRAY(p, 3)) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_BatchSD::print_config() const
{
  LMark;
  LoomLogPlat::print_config();
  LMark;
  LPrintln("\tCurrent Batch     : ", batch_counter);
  LMark;
  LPrintln("\tCurrent Packet    : ", packet_counter);
  LMark;
  LPrintln("\tPacket Drops      : ", drop_count);
  LMark;
  LPrintln("\tPacket Drop Rate  : ", get_drop_rate());
  LMark;
}

///////////////////////////////////////////////////////////////////////////////
bool Loom_BatchSD::dump_batch(int index){
  LMark;
  #if LOOM_DEBUG == 1
    LMark;
    char file_name[30];
    LMark;
    create_file_name(index, file_name);
    LMark;
    //digitalWrite(8, HIGH); // if using LoRa, need to temporarily prevent it from using SPI

    //sd.begin(chip_select); // It seems that SD card may become 'unsetup' sometimes, so re-setup


    File file = sd.open(file_name, O_READ);
    LMark;

    if (file) {
      LMark;
      print_module_label();
      LMark;
      LPrintln("Contents of file: ", file_name);
      LMark;

      // read from the file until there's nothing else in it:
      while (file.available())
        Serial.write(file.read());
      LMark;
      LPrintln();
      LMark;
      file.close();
      LMark;
      return true;
    } else {
      // if the file didn't open, print an error:
      print_module_label();
      LMark;
      LPrintln("Error opening ", file_name);
      LMark;
      return false;
    }
  #endif
  LMark;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_BatchSD::clear_batch_log(){
  LMark;
  print_module_label();
  LMark;
  LPrintln("Clearing batch log");
  LMark;
  char file_name[30];
  LMark;
  digitalWrite(8, HIGH); // if using LoRa, need to temporarily prevent it from using SPI
  LMark;
  //sd.begin(chip_select);

  // For all the packets stored in the batch, create the file name and remove from SD
  for(int i=0; i<packet_counter;i++){
    LMark;
    create_file_name(i, file_name);
    LMark;
    sd.remove(file_name);
    LMark;
  }

  // Reset packet counter and increment to next batch
  packet_counter = 0;
  LMark;
  drop_count = 0;
  LMark;
  batch_counter++;
  LMark;
}

///////////////////////////////////////////////////////////////////////////////
bool Loom_BatchSD::store_batch(){
  LMark;
  JsonObject obj = device_manager->internal_json(false);
  LMark;
  if(obj.isNull() || obj.size()==0) return false;
  LMark;
  return store_batch_json(obj);
}

///////////////////////////////////////////////////////////////////////////////
bool Loom_BatchSD::store_batch_json(JsonObject json){
  LMark;
  // Create file name and add which Batch the packet is from to json
  char file_name[30];
  LMark;
  create_file_name(packet_counter, file_name);
  LMark;
  digitalWrite(8, HIGH); // if using LoRa, need to temporarily prevent it from using SPI
  LMark;
  //sd.begin(chip_select);

  // Write json to batch file
  File file;
  LMark;
  if( (!file.open(file_name, O_WRONLY | O_CREAT)) ){
    LMark;
    print_module_label();
    LMark;
    LPrintln("Error opening: ", file_name);
    LMark;
    return false;
  }
  else {
    LMark;
    print_module_label();
    LMark;
    LPrintln("Writing to: ", file_name);
    LMark;
  }

  serializeJson(json, file);
  LMark;

  if (!file.sync()) {
    LMark;
		file.close();
    LMark;
		print_module_label();
    LMark;
		LPrintln("Sync Error");
    LMark;
		return false;
	}
  else if(file.getWriteError()){
    LMark;
    file.close();
    LMark;
    print_module_label();
    LMark;
    LPrintln("Write Error");
    LMark;
    return false;
  }
  file.close();
  LMark;
  packet_counter++;
  LMark;
  print_module_label();
  LMark;
  LPrintln("Done writing to Batch");
  LMark;
  return true;
}

///////////////////////////////////////////////////////////////////////////////
JsonObject Loom_BatchSD::get_batch_json(int index){
  LMark;
  doc.clear();
  LMark;

  char file_name[30];
  LMark;
  create_file_name(index, file_name);
  LMark;

  digitalWrite(8, HIGH); // if using LoRa, need to temporarily prevent it from using SPI
  LMark;
  //sd.begin(chip_select);

  if(index <= packet_counter || index < 0){
    LMark;
    print_module_label();
    LMark;
    LPrintln("Retrieving Packet in file ", file_name);
    LMark;
    File file = sd.open(file_name, O_READ);
    LMark;
    if(!file) {
      LMark;
      print_module_label();
      LMark;
      LPrintln("Failed to open '", file_name, "'");
      LMark;
    }

    DeserializationError error = deserializeJson(doc, file);
    LMark;
    if(error) {
      LMark;
      print_module_label();
      LMark;
      LPrintln("deserializeJson() failed: ", error.c_str());
      LMark;
    }

    else {
      LMark;
      return doc.as<JsonObject>();
    }
  }
  else {
    LMark;
    print_module_label();
    LMark;
    LPrintln("Get Batch Error: Index is out of range");
    LMark;
  }
}

///////////////////////////////////////////////////////////////////////////////
void Loom_BatchSD::power_up() {
  LMark;
  digitalWrite(8, HIGH);
  LMark;
  sd.begin(chip_select, SD_SCK_MHZ(50));
  LMark;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_BatchSD::power_down(){
  LMark;
  //do nothing
}

///////////////////////////////////////////////////////////////////////////////
void Loom_BatchSD::create_file_name(int index, char* name){
  LMark;
  String file = "Batches/Batch-";
  LMark;
  file = file + batch_counter;
  LMark;
  file = file + "-";
  LMark;
  file = file + index;
  LMark;
  file = file + ".json";
  LMark;
  file.toCharArray(name, file.length()+1);
  LMark;
}

///////////////////////////////////////////////////////////////////////////////
float Loom_BatchSD::get_drop_rate() const
{
	return (packet_counter == 0)
		? 0.0f
		: static_cast<float>(drop_count)*100.0f/static_cast<float>(packet_counter);
}

///////////////////////////////////////////////////////////////////////////////
void Loom_BatchSD::package(JsonObject json) {
  LMark;
  JsonObject data = get_module_data_object(json, "Batch");
  LMark;

  data["Number"] = batch_counter+1;
  LMark;
}
