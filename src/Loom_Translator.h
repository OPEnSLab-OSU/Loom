///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_Translator.h
/// @brief		File for declarations of general purpose functions to convert the 
///				type of single values or arrays
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Loom_Misc.h"
#include <Arduino.h>


// ================================================================
// ===             CONVERSION BETWEEN ARRAY FORMATS             ===
// ================================================================


/// Converts key-value array to pair of associated key and value arrays.
/// Convert incoming array formatted as:
///   [key1, value1, key2, value2, key3, value3]
/// to two associated arrays formatted as:
///   [key1,   key2,   key3]
///   [value1, value2, value3]
///
/// @param[in]	key_values	The flat array of keys and values
/// @param[out]	keys		The array to be filled with the keys from 'key_values'
/// @param[out]	values		The array to be filled with the values for 'key_values'
/// @param[in]	kv_len		The length of the 'key_value' array
/// @param[in]	assoc_len	The length of the 'keys' and 'values' arrays (should be the same)
void convert_array_key_value_to_assoc(String key_values [], String keys [], String values [], int kv_len, int assoc_len);

/// Converts pair of associated key and value arrays to equivalent key-value array.
/// Converts incoming associated arrays formatted as:
///   [key1,   key2,   key3]
///   [value1, value2, value3]:
/// to single array formatted as:
///   [key1, value1, key2, value2, key3, value3]
///
/// @param[in]	keys		The array of keys
/// @param[in]	values		The array to values
/// @param[out]	key_values	The flat array of keys and values to be filled by combining 'keys' and 'values'
/// @param[in]	assoc_len	The length of the 'keys' and 'values' arrays (should be the same)
/// @param[in]	kv_len		The length of the 'key_value' array
void convert_array_assoc_to_key_value(String keys [], String values [], String key_values [], int assoc_len, int kv_len);

/// Overloaded version of convert_array_assoc_to_key_value to convert output type.
/// Converts incoming associated arrays formatted as:
///   [key1,   key2,   key3]
///   [value1, value2, value3]:
/// to single array formatted as:
///   [key1, value1, key2, value2, key3, value3]
///
/// @param[in]	keys		The array of keys
/// @param[in]	values		The array to values
/// @param[out]	key_values	The flat array of keys and values to be filled by combining 'keys' and 'values'
/// @param[in]	assoc_len	The length of the 'keys' and 'values' arrays (should be the same)
/// @param[in]	kv_len		The length of the 'key_value' array
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



// ================================================================
// ===              CONVERSION BETWEEN DATA TYPES               ===
// ================================================================

/// Convert a c-string to int.
/// @param[in]	s	C-string to try to convert
/// @return 		The converted int
int			convert_string_to_int(const char* s);
/// Convert a String to float.
/// @param[in]	s	C-string to try to convert
/// @return 		The converted float
float		convert_string_to_float(const char* s);
/// Convert a String to int.
/// @param[in]	s	C-string to try to convert
/// @return 		The converted int
int			convert_string_to_int(String s);
/// Convert a String to float.
/// @param[in]	s	C-string to try to convert
/// @return 		The converted float
float		convert_string_to_float(String s);



// ================================================================
// ===              CONVERSION BETWEEN ARRAY TYPES              ===
// ================================================================

// --- Conversion between array element types ---
// void convert_array(String src [], int    dest [],   int count);
// void convert_array(String src [], float  dest [],   int count);
// void convert_array(String src [], char dest [][20], int count);


/// Converts array of generic type to array Strings
/// @param[in]	src		Incoming array
/// @param[out]	dest	Outgoing array
/// @param[in]	count	Number of elements in array
template <typename Tin>
void		convert_array(Tin src[], String dest[], int count)
{
	for (int i = 0; i < count; i++) {
		dest[i] = String(src[i]);
	}
}

/// Converts array of generic type to array of different provided type
/// @param[in]	src		Incoming array
/// @param[out]	dest	Outgoing array
/// @param[in]	count	Number of elements in array
template <typename Tin, typename Tout>
void		convert_array(Tin src [], Tout dest[], int count)
{
	String tmps[count];
	convert_array(src, tmps, count);
	convert_array(tmps, dest, count);
}


