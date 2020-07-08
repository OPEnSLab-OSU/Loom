// You are happy to change the file name(and we recommend it)
// However, if you are going to change the name of both .cpp and .h file,
// make sure that you change the code where it says [#include "LoomJSON.h"]
// to #include "[name of the .h file]"

#include "LoomJSON.h"

///////////////////////////////////////////////////////////////////////////////////////////////

// Do not modify the code below! Scroll Down Please!

///////////////////////////////////////////////////////////////////////////////////////////////

static bool get_data_point_from_contents(const JsonArrayConst& contents, const char* module_name, const char* data_key, float& out){
    for(const JsonVariantConst& module_data: contents){
        const char* name = module_data["module"];
        if (name == nullptr){
            continue;
        }
        if (strncmp(name, module_name, 15) != 0){
            continue;
        }

        const JsonObjectConst data_obj = module_data["data"];

        if (data_obj.isNull()){
            continue;
        }

        const JsonVariantConst data_value = data_obj[data_key];

        if (!data_value.isNull() && data_value.is<float>()){
            out = data_value.as<float>();
            return true;
        }
    }

    out = NAN;
    return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////

// Do not modify the code below! Scroll Down Please!

///////////////////////////////////////////////////////////////////////////////////////////////

static bool get_data_point_from_contents_int(const JsonArrayConst& contents, const char* module_name, const char* data_key, int& out) {
  for (const JsonVariantConst& module_data : contents) {

    const char* name = module_data["module"];
    if (name == nullptr)

      continue;

    if (strncmp(name, module_name, 15) != 0)
      continue;

    const JsonObjectConst data_obj = module_data["data"];

    if (data_obj.isNull())
      continue;

    const JsonVariantConst data_value = data_obj[data_key];
    if (!data_value.isNull() && data_value.is<int>()) {

      out = data_value.as<int>();

      return true;
    }
  }

  out = -3333;
  return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////

// This function below will change the JSON to struct, which is in binary code.

// In other words, the data that you measure will convert from readable values 
// to unreadable values.

// The reason why we convert that binary code(unreadable values) 
// is because it hold more data information than JSON (readable values).

//                          Things to know to modify the function
// 1. Make sure what type of values that the sensor returns.
//    i.e. If it returns as a whole number, it will properly int(except K30).
//         If not, then it is a float

// 2. Make sure what is the order that prints in the config. 
// For example, like this.

//{
//  "type": "data",
//  "id": {
//    "name": "LoRa_Raw_Transmit",
//    "instance": 1
//  },
//  "contents": [
//    {
//      "module": "Packet",
//      "data": {
//       "Number": 4
//     }
//    },
//    {
//      "module": "SHT31D",
//      "data": {
//        "temp": 29.48, // This is a float variable
//        "humid": 58.53
//      }
//    },
//    {
//      "module": "TSL2591",
//      "data": {
//        "Vis": 108,  // This is an int variable
//        "IR": 38,
//        "Full": 146
//      }
//    },

// Note that you need to know what order comes first: not just the sensor order,
// but also the sensor value order: TSL2591 sensor values go Vis, IR, then Full. 

//                          Things to modify in this function!

// 1. Change the name from Loom_Base to something you want to use.
//    However, this name must also match the files in Transmit_Raw.ino and LoomJSON.h 

// 2. Insert the following code where it says "From here, you should modify"

    // 2.a Make sure you insert the code how config file prints out: It is very Important

    // 2.b If the return value is an int, then write get_data_point_from_contents_int

    // 2.c If not, the write get_data_point_from_contents

    // 2.d Modify the sensor name, and sensor value name in the second and third part
    //     as the example is shown.

    // 2.e Modify the last part as this: "out.data.[name]"
    //     While the [name] will be the name that you wrote in LoomJSON.h file or your .h file
    //     Like how the example code shows

///////////////////////////////////////////////////////////////////////////////////////////////

void json_to_struct(const JsonObjectConst& data, Loom_Base& out) {
  // You don't need to modify this part.
  // name, instance
  const JsonObjectConst id = data["id"];
  if (!id.isNull()) {
    const char* name = id["name"];
    if (name != nullptr)
      strncpy(out.data.name, name, 15);
    else
      out.data.name[0] = 0;
    out.data.instance = id["instance"] | -1;
  }

  // You don't need to modify this part. 
  // get a reference to the "contents" object
  const JsonArrayConst contents = data["contents"];
  if (contents.isNull())
    return;
  // find every data point we care about, and populate the struct with it

  // You don't need to modify this part. 
  // packet #
  get_data_point_from_contents_int(contents, "Packet", "Number", out.data.packageNum);

  // From here, you should modify.

  // STEMMA Moisture
  get_data_point_from_contents_int(contents, "STEMMA", "capactive", out.data.stemmaMositure);

  // STEMMA temperature
  get_data_point_from_contents(contents, "STEMMA", "temperature", out.data.stemmaTemp);

  // SHT31D temperature
  get_data_point_from_contents(contents, "SHT31D", "temp", out.data.sht31dTemp);
    
  // SHT31D humidity
  get_data_point_from_contents(contents, "SHT31D", "humid", out.data.sht31dHumidity);

  // TSL2591 Vis
  get_data_point_from_contents_int(contents, "TSL2591", "Vis", out.data.tsl2591Vis);

  // TSL2591 IR
  get_data_point_from_contents_int(contents, "TSL2591", "IR", out.data.tsl2591IR);

  // TSL2591 Full
  get_data_point_from_contents_int(contents, "TSL2591", "Full", out.data.tsl2591Full);


  // You don't need to modify this part. 
  // timestamp
  const JsonObjectConst stamp = data["timestamp"];
  if (!stamp.isNull()) {
    const char* date = stamp["date"].as<const char*>();
    if (date != nullptr)
      strncpy(out.data.datestamp, date, 15);
    else
      out.data.datestamp[0] = 0;
    const char* time = stamp["time"].as<const char*>();
    if (time != nullptr)
      strncpy(out.data.timestamp, time, 15);
    else
      out.data.timestamp[0] = 0;
  }
}

/////////////////////////////////////////////////////////////////////////////////////////

// Do not modify the code below! Scroll Down Please!

//////////////////////////////////////////////////////////////////////////////////////////

static JsonObject make_module_object(const JsonArray& contents, const char* name) {
  const JsonObject data = contents.createNestedObject();
  data["module"] = name;
  return data.createNestedObject("data");
}

///////////////////////////////////////////////////////////////////////////////////////////////

// While the previous function JSON to binary, this function is the opposite.
// Make it readable from unreadable code.

//                          Things to modify in this function!

// 1. Change the name from Loom_Base to something you want to use.
//    However, this name must also match the files in Transmit_Raw.ino and LoomJSON.h

// 2. Insert the following code after it says "You should modify."

    // 2.a To make a new module, make a new {} under Package Number {} part

    // 2.b Inside the {}, write const JsonObject data = make_module_object(contents, "[name]");
    //     While the [name] will be the Sensor name. Make sure to follow the config.h print order

    // 2.c Under that, write data[name] = in.data.(Name);
    //     While [name] is the Sensor value name 
    //     and (Name) is the name that you wrote in LoomJSON.h file or your .h file


///////////////////////////////////////////////////////////////////////////////////////////////

void struct_to_json(const Loom_Base& in, const JsonObject& out) {
  // You don't need to modify this part.
  // start adding object to it!
  out["type"] = "data";

  // You don't need to modify this part.
  // id block
  const JsonObject id = out.createNestedObject("id");
  id["name"] = in.data.name;
  id["instance"] = in.data.instance;

  // You don't need to modify this part.
  // timestamp block
  const JsonObject timestamp = out.createNestedObject("timestamp");
  timestamp["date"] = in.data.datestamp;
  timestamp["time"] = in.data.timestamp;

  // You don't need to modify this part.
  // contents array
  const JsonArray contents = out.createNestedArray("contents");
  // add objects for each sensor!

  // You don't need to modify this part.
  // pkt number
  {
    const JsonObject data = make_module_object(contents, "Packet");
    data["Number"] = in.data.packageNum;
  }

  // You should modify.
  // stemma
  {
      const JsonObject data = make_module_object(contents, "STEMMA");
      data["capactive"] = in.data.stemmaMositure;
      data["temperature"] = in.data.stemmaTemp;
  }

  // You should modify.
  // sht31d
  {
    const JsonObject data = make_module_object(contents, "SHT31D");
    data["temp"] = in.data.sht31dTemp;
    data["humid"] = in.data.sht31dHumidity;
    
  }
  // You should modify.
  //tsl2591
  {
    const JsonObject data = make_module_object(contents, "TSL2591");
    data["Vis"] = in.data.tsl2591Vis;
    data["IR"] = in.data.tsl2591IR;
    data["Full"] = in.data.tsl2591Full;
  }

}
