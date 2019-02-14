
#ifndef LOOM_TRANSLATOR_h
#define LOOM_TRANSLATOR_h

#include "Loom_Misc.h"

#include <OSCBundle.h>


enum BundleStructure { SINGLEMSG, MULTIMSG };



// Conversions between bundles and strings (and auxiliary functions)
void convert_OSC_string_to_bundle(char *osc_string, OSCBundle*bndl);
void convert_OSC_bundle_to_string(OSCBundle *bndl, char *osc_string);
void original_convert_OSC_string_to_bundle(char *osc_string, OSCBundle*bndl);
void original_convert_OSC_bundle_to_string(OSCBundle *bndl, char *osc_string);
void uncompress_OSC_string(char *osc_string);
void str_replace(char *target, const char *needle, const char *replacement);
const char* nth_strchr(const char* s, char c, int n);

// Conversion between bundle formats
void convert_bundle_structure(OSCBundle *bndl, OSCBundle *outBndl, BundleStructure format);
void convert_bundle_structure(OSCBundle *bndl, BundleStructure format);


// Conversion from bundle to array formats
void convert_bundle_to_array_key_value(OSCBundle *bndl, String key_values[], int kv_len);
void convert_bundle_to_arrays_assoc(OSCBundle *bndl, String keys[], String values[], int assoc_len);

template <typename T>
void convert_bundle_to_array(OSCBundle *bndl, T data [], int len);
void convert_bundle_to_array_w_header(OSCBundle *bndl, String data [], int len);


// Conversion from array to bundle formats
void convert_key_value_array_to_bundle(String key_values [], OSCBundle *bndl, char packet_header[], int kv_len, BundleStructure format, int interpret);
void convert_key_value_array_to_bundle(String key_values [], OSCBundle *bndl, char packet_header[], int kv_len, BundleStructure format);

void convert_assoc_arrays_to_bundle(String keys [], String values [], OSCBundle *bndl, char packet_header[], int assoc_len, BundleStructure format, int interpret);
template <typename T>
void convert_assoc_arrays_to_bundle(String keys [], T values [], OSCBundle *bndl, char packet_header[], int assoc_len, BundleStructure format);

template <typename T>
void convert_array_to_bundle(T data [], OSCBundle *bndl, char packet_header[], int len);


// Conversion between array formats
void convert_array_key_value_to_assoc(String key_values [], String keys [], String values [], int kv_len, int assoc_len);
void convert_array_assoc_to_key_value(String keys [], String values [], String key_values [], int assoc_len, int kv_len);
template <typename T>
void convert_array_assoc_to_key_value(String keys [], T values [], String key_values [], int assoc_len, int kv_len);


// Convert strings to numbers
int    convert_string_to_int(char * s);
float  convert_string_to_float(char * s);
int    convert_string_to_int(String s);
float  convert_string_to_float(String s);


// Conversion between array element types
void convert_array(String src [], int    dest [],   int count);
void convert_array(String src [], float  dest [],   int count);
void convert_array(String src [], char dest [][20], int count);
template <typename Tin> 
void convert_array(Tin src[], String dest[], int count);
template <typename Tin, typename Tout> 
void convert_array(Tin src [], Tout dest[], int count);




#endif // of #ifndef LOOM_TRANSLATOR_h

