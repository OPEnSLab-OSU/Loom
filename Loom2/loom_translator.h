
#ifndef LOOM_TRANSLATOR_h
#define LOOM_TRANSLATOR_h

#include "Loom_Misc.h"

#include <OSCBundle.h>



// --- Converts a bundle with multiple messages to a bundle with only one message ---
void flatten_bundle(OSCBundle& bndl, OSCBundle& out_bndl);
void flatten_bundle(OSCBundle& bndl);


// --- Conversion from bundle to array formats---
void convert_bundle_to_array_key_value(OSCBundle& bndl, String key_values[], int kv_len);
void convert_bundle_to_arrays_assoc(OSCBundle& bndl, String keys[], String values[], int assoc_len);


// --- Convert bundle to array of given type, converting data elements to type T ---
template <typename T>
void convert_bundle_to_array(OSCBundle& bndl, T data [], int len)
{
	String tmp_strings[len];
	convert_bundle_to_array_key_value(bndl, tmp_strings, len);
	convert_array(tmp_strings, data, len);
}



// --- Conversion between array formats ---
void convert_array_key_value_to_assoc(String key_values [], String keys [], String values [], int kv_len, int assoc_len);

void convert_array_assoc_to_key_value(String keys [], String values [], String key_values [], int assoc_len, int kv_len);

template <typename T>
void convert_array_assoc_to_key_value(String keys [], T values [], String key_values [], int assoc_len, int kv_len)
{
	if ( kv_len < 2*assoc_len ) {
		#if LOOM_DEBUG == 1
			Serial.println("Key-values array is not at least twice the size of keys and values arrays, cannot merge");
		#endif
		return;
	}

	for (int i = 0; i < assoc_len; i++) {
		key_values[i*2]   = keys[i];
		key_values[i*2+1] = String(values[i]);
	}
}




// --- Conversion from array to bundle formats ---
void convert_key_value_array_to_bundle(String key_values [], OSCBundle& bndl, char* address, int kv_len, int interpret=0);

// void convert_assoc_arrays_to_bundle(String keys [], String values [], OSCBundle& bndl, char* address, int assoc_len, int interpret=0);


template <typename T>
void convert_assoc_arrays_to_bundle(String keys [], T values [], OSCBundle& bndl, char* address, int assoc_len, int interpret=0)
{
	String converted_values[assoc_len];
	convert_array(values, converted_values, assoc_len);
	convert_assoc_arrays_to_bundle(keys, converted_values, bndl, address, assoc_len, interpret);

}









// --- Conversion from array of non-Strings to single message bundle ---
template <typename T>   
void convert_array_to_bundle(T data [], OSCBundle& bndl, char* address, int len)
{
	bndl.empty();
	bndl.add(address);
	for (int i = 0; i < len; i++) {
		append_to_bundle(bndl, address, i, data[i]);
	}
}



// --- Convert strings to numbers ---
int    convert_string_to_int(char* s);
float  convert_string_to_float(char* s);
int    convert_string_to_int(String s);
float  convert_string_to_float(String s);




// --- Conversion between array element types ---
// void convert_array(String src [], int    dest [],   int count);
// void convert_array(String src [], float  dest [],   int count);
// void convert_array(String src [], char dest [][20], int count);

template <typename Tin> 
void convert_array(Tin src[], String dest[], int count)
{ 
	for (int i = 0; i < count; i++) {
		dest[i] = String(src[i]); 
	}
}

template <typename Tin, typename Tout> 
void convert_array(Tin src [], Tout dest[], int count)
{ 
	String tmps[count]; 
	convert_array(src, tmps, count); 
	convert_array(tmps, dest, count); 
}



#endif // of #ifndef LOOM_TRANSLATOR_h

