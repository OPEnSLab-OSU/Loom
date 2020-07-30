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
  digitalWrite(8, HIGH); // if using LoRa, need to temporarily prevent it from using SPI

  bool sd_found = sd.begin(chip_select, SD_SCK_MHZ(50));
  batch_counter=0;
  packet_counter=0;
  drop_count = 0;
  if(sd_found) {
    // Setup Directory for Batch Files
    sd.mkdir("Batches");
  }

  print_module_label();
  LPrintln("Initialize ", (sd_found) ? "sucessful" : "failed");

}

///////////////////////////////////////////////////////////////////////////////
Loom_BatchSD::Loom_BatchSD(LoomManager* manager, JsonArrayConst p)
  : Loom_BatchSD(manager,EXPAND_ARRAY(p, 3)) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_BatchSD::print_config() const
{
  LoomLogPlat::print_config();
  LPrintln("\tCurrent Batch     : ", batch_counter);
  LPrintln("\tCurrent Packet    : ", packet_counter);
  LPrintln("\tPacket Drops      : ", drop_count);
  LPrintln("\tPacket Drop Rate  : ", get_drop_rate());
}

///////////////////////////////////////////////////////////////////////////////
bool Loom_BatchSD::dump_batch(int index){
  #if LOOM_DEBUG == 1
    char file_name[30];
    create_file_name(index, file_name);
    //digitalWrite(8, HIGH); // if using LoRa, need to temporarily prevent it from using SPI

    //sd.begin(chip_select); // It seems that SD card may become 'unsetup' sometimes, so re-setup


    File file = sd.open(file_name, O_READ);

    if (file) {
      print_module_label();
      LPrintln("Contents of file: ", file_name);

      // read from the file until there's nothing else in it:
      while (file.available())
        Serial.write(file.read());
      LPrintln();
      file.close();
      return true;
    } else {
      // if the file didn't open, print an error:
      print_module_label();
      LPrintln("Error opening ", file_name);
      return false;
    }
  #endif
}

///////////////////////////////////////////////////////////////////////////////
void Loom_BatchSD::clear_batch_log(){
  print_module_label();
  LPrintln("Clearing batch log");
  char file_name[30];
  digitalWrite(8, HIGH); // if using LoRa, need to temporarily prevent it from using SPI
  //sd.begin(chip_select);

  // For all the packets stored in the batch, create the file name and remove from SD
  for(int i=0; i<packet_counter;i++){
    create_file_name(i, file_name);
    sd.remove(file_name);
  }

  // Reset packet counter and increment to next batch
  packet_counter = 0;
  drop_count = 0;
  batch_counter++;
}

///////////////////////////////////////////////////////////////////////////////
bool Loom_BatchSD::store_batch(){
  JsonObject obj = device_manager->internal_json(false);
  if(obj.isNull() || obj.size()==0) return false;
  return store_batch_json(obj);
}

///////////////////////////////////////////////////////////////////////////////
bool Loom_BatchSD::store_batch_json(JsonObject json){
  // Create file name and add which Batch the packet is from to json
  char file_name[30];
  create_file_name(packet_counter, file_name);
  digitalWrite(8, HIGH); // if using LoRa, need to temporarily prevent it from using SPI
  //sd.begin(chip_select);

  // Write json to batch file
  File file;
  if( (!file.open(file_name, O_WRONLY | O_CREAT)) ){
    print_module_label();
    LPrintln("Error opening: ", file_name);
    return false;
  }
  else {
    print_module_label();
    LPrintln("Writing to: ", file_name);
  }

  serializeJson(json, file);

  if (!file.sync()) {
		file.close();
		print_module_label();
		LPrintln("Sync Error");
		return false;
	}
  else if(file.getWriteError()){
    file.close();
    print_module_label();
    LPrintln("Write Error");
    return false;
  }
  file.close();
  packet_counter++;
  print_module_label();
  LPrintln("Done writing to Batch");
  return true;
}

///////////////////////////////////////////////////////////////////////////////
JsonObject Loom_BatchSD::get_batch_json(int index){
  doc.clear();

  char file_name[30];
  create_file_name(index, file_name);

  digitalWrite(8, HIGH); // if using LoRa, need to temporarily prevent it from using SPI
  //sd.begin(chip_select);

  if(index <= packet_counter || index < 0){
    print_module_label();
    LPrintln("Retrieving Packet in file ", file_name);
    File file = sd.open(file_name, O_READ);
    if(!file) {
      print_module_label();
      LPrintln("Failed to open '", file_name, "'");
    }

    DeserializationError error = deserializeJson(doc, file);
    if(error) {
      print_module_label();
      LPrintln("deserializeJson() failed: ", error.c_str());
    }

    else {
      return doc.as<JsonObject>();
    }
  }
  else {
    print_module_label();
    LPrintln("Get Batch Error: Index is out of range");
  }
}

///////////////////////////////////////////////////////////////////////////////
void Loom_BatchSD::power_up() {
  digitalWrite(8, HIGH);
  sd.begin(chip_select, SD_SCK_MHZ(50));
}

///////////////////////////////////////////////////////////////////////////////
void Loom_BatchSD::power_down(){
  //do nothing
}

///////////////////////////////////////////////////////////////////////////////
void Loom_BatchSD::create_file_name(int index, char* name){
  String file = "Batches/Batch-";
  file = file + batch_counter;
  file = file + "-";
  file = file + index;
  file = file + ".json";
  file.toCharArray(name, file.length()+1);
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
  JsonObject data = get_module_data_object(json, "Batch");

  data["Number"] = batch_counter+1;
}
