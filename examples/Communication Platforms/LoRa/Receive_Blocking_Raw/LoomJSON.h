/////////////////////////////////////////////////////////////////////////////////////////

// Do not modify the code below! Scroll Down Please!

//////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <stdint.h>
#include <ArduinoJson.h> 


/////////////////////////////////////////////////////////////////////////////////////////

// This code below is where the LoomJSON.h variable name came from.
// Make sure the name of the variable matches the variable name in the LoomJSON.cpp
// in the certain location.

// Also, you are allow to change the name(and we recommend) from Loom_Base_t to 
// something else. 
// Note that Loom_Base_t has a different name as Loom_Base(This will be used more often)

//////////////////////////////////////////////////////////////////////////////////////////

struct Loom_Base_t
{
    // You don't need to modify the code below.
    char name[16];
    int instance;
    char datestamp[16];
    char timestamp[16];
    int packageNum;

    // From here, you need to modify it.
    int stemmaMositure;
    float stemmaTemp;
    float sht31dHumidity;
    float sht31dTemp;
    int tsl2591Vis;
    int tsl2591IR;
    int tsl2591Full;
};

//////////////////////////////////////////////////////////////////////////////////////////

// If you change the name of Loom_Base_t and Loom_Base, 
// make sure you change the names in the following code. 

//////////////////////////////////////////////////////////////////////////////////////////

typedef union {
    Loom_Base_t data;
    uint8_t raw[sizeof(Loom_Base_t)];
} Loom_Base;

void json_to_struct(const JsonObjectConst& data, Loom_Base& out);

void struct_to_json(const Loom_Base& in, const JsonObject& out);