
#ifndef LOOM_TRANSLATOR_h
#define LOOM_TRANSLATOR_h

#include "Loom_Misc.h"
#include <OSCBundle.h>


// ================================================================
// ===                      FLATTEN BUNDLE                      ===
// ================================================================

/// Take any bundle, output bundle with only one message.
/// \param[in]	bndl		Input bundle
/// \param[out]	out_bndl	Flattened bundle
void		flatten_bundle(OSCBundle& bndl, OSCBundle& out_bndl);

/// Converts a bundle with to one with only one message in place.
/// \param[in,out]	bndl	Bundle to flatten
void		flatten_bundle(OSCBundle& bndl);


// ================================================================
// ===         CONVERSION FROM BUNDLE TO ARRAY FORMATS          ===
// ================================================================


/// Converts an OSC bundle to an equivalent array.
/// Formatted as [key1, value1, key2, value2, key3, value3] 
/// \param[in]	bndl		The bundle to use to fill the key value array
/// \param[out]	key_values	The array to be populated
/// \param[in]	kv_len		The length of the key_values array
void		convert_bundle_to_array_key_value(OSCBundle& bndl, String key_values[], int kv_len);

/// Converts an OSC bundle to equivalent pair of associated arrays.
/// formated as:
///   [key1,   key2,   key3]
///   [value1, value2, value3]
/// Converts bundle to flat single message if not already
/// \param[in]	bndl		The bundle to use to fill the arrays
/// \param[out]	keys		The array of keys be populated
/// \param[out]	values		The array of values to be populated
/// \param[in]	assoc_len	The length of the incoming arrays
void		convert_bundle_to_arrays_assoc(OSCBundle& bndl, String keys[], String values[], int assoc_len);


/// Convert bundle to array of given type, converting data elements to type T.
/// \param[in]	bndl	The bundle to read
/// \param[out]	data	The array to fill
/// \param[in]	len		The length of the data array
template <typename T>
void		convert_bundle_to_array(OSCBundle& bndl, T data [], int len)
{
	String tmp_strings[len];
	convert_bundle_to_array_key_value(bndl, tmp_strings, len);
	convert_array(tmp_strings, data, len);
}



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
/// \param[in]	key_values	The flat array of keys and values
/// \param[out]	keys		The array to be filled with the keys from 'key_values'
/// \param[out]	values		The array to be filled with the values for 'key_values'
/// \param[in]	kv_len		The length of the 'key_value' array
/// \param[in]	assoc_len	The length of the 'keys' and 'values' arrays (should be the same)
void		convert_array_key_value_to_assoc(String key_values [], String keys [], String values [], int kv_len, int assoc_len);

/// Converts pair of associated key and value arrays to equivalent key-value array.
/// Converts incoming associated arrays formatted as:
///   [key1,   key2,   key3]
///   [value1, value2, value3]:
/// to single array formatted as:
///   [key1, value1, key2, value2, key3, value3]
/// 
/// \param[in]	keys		The array of keys  
/// \param[in]	values		The array to values
/// \param[out]	key_values	The flat array of keys and values to be filled by combining 'keys' and 'values'
/// \param[in]	assoc_len	The length of the 'keys' and 'values' arrays (should be the same)
/// \param[in]	kv_len		The length of the 'key_value' array
void		convert_array_assoc_to_key_value(String keys [], String values [], String key_values [], int assoc_len, int kv_len);


/// Overloaded version of convert_array_assoc_to_key_value to convert output type.
/// Converts incoming associated arrays formatted as:
///   [key1,   key2,   key3]
///   [value1, value2, value3]:
/// to single array formatted as:
///   [key1, value1, key2, value2, key3, value3]
/// 
/// \param[in]	keys		The array of keys  
/// \param[in]	values		The array to values
/// \param[out]	key_values	The flat array of keys and values to be filled by combining 'keys' and 'values'
/// \param[in]	assoc_len	The length of the 'keys' and 'values' arrays (should be the same)
/// \param[in]	kv_len		The length of the 'key_value' array
template <typename T>
void		convert_array_assoc_to_key_value(String keys [], T values [], String key_values [], int assoc_len, int kv_len)
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
// ===         CONVERSION FROM ARRAY TO BUNDLE FORMATS          ===
// ================================================================

/// Converts key-value array to equivalent bundle.
/// Convert an array formatted as:
///   [key1, value1, key2, value2 ...]
/// to an OSC bundle with single message
/// 
/// \param[in]	keys_values		The flat array of keys and values  
/// \param[out]	bndl			The flat array of keys and values to be filled by combining 'keys' and 'values'
/// \param[in]	address			The address to add to bundle/messages 
/// \param[in]	kv_len			The length of the 'keys_values' array 
/// \param[in]	interpret		(see comment at start of section above for details about the parameter)
void		convert_key_value_array_to_bundle(String key_values [], OSCBundle& bndl, char* address, int kv_len, int interpret=0);



// void convert_assoc_arrays_to_bundle(String keys [], String values [], OSCBundle& bndl, char* address, int assoc_len, int interpret=0);



/// Converts an associated pair of key and value arrays to equivalent bundle.
/// Convert an array formatted as:
///   [key1, key2 ...]
///   [value1, value2 ...]
/// to an OSC message with single message
///
/// \param[in]	keys		The array of keys  
/// \param[in]	values		The array to values
/// \param[out]	bndl		The flat array of keys and values to be filled by combining 'keys' and 'values'
/// \param[in]	address		The address to add to bundle/messages 
/// \param[in]	assoc_len	The length of the 'keys' and 'values' arrays (should be the same)
/// \param[in]	interpret	(see comment at start of section above for details about the parameter)
template <typename T>
void		convert_assoc_arrays_to_bundle(String keys [], T values [], OSCBundle& bndl, char* address, int assoc_len, int interpret=0)
{
	String converted_values[assoc_len];
	convert_array(values, converted_values, assoc_len);
	convert_assoc_arrays_to_bundle(keys, converted_values, bndl, address, assoc_len, interpret);

}


// ================================================================
// ===              CONVERSION BETWEEN DATA TYPES               ===
// ================================================================

/// Converts array of general type array to single message bundle.
///
/// \param[in]	data	Incoming data to create bundle from
/// \param[out]	bndl	The bundle to populate
/// \param[in]	address	The OSC address to add to bundle
/// \param[in]	len		The length of the incoming array
template <typename T>   
void		convert_array_to_bundle(T data [], OSCBundle& bndl, char* address, int len)
{
	bndl.empty();
	bndl.add(address);
	for (int i = 0; i < len; i++) {
		append_to_bundle(bndl, address, i, data[i]);
	}
}

// ================================================================
// ===              CONVERSION BETWEEN DATA TYPES               ===
// ================================================================

/// Convert a c-string to int.
/// \param[in]	s	C-string to try to convert
/// \return 		The converted int
int			convert_string_to_int(char* s);
/// Convert a String to float.
/// \param[in]	s	C-string to try to convert
/// \return 		The converted float
float		convert_string_to_float(char* s);
/// Convert a String to int.
/// \param[in]	s	C-string to try to convert
/// \return 		The converted int
int			convert_string_to_int(String s);
/// Convert a String to float.
/// \param[in]	s	C-string to try to convert
/// \return 		The converted float
float		convert_string_to_float(String s);

// ================================================================
// ===              CONVERSION BETWEEN ARRAY TYPES              ===
// ================================================================



// --- Conversion between array element types ---
// void convert_array(String src [], int    dest [],   int count);
// void convert_array(String src [], float  dest [],   int count);
// void convert_array(String src [], char dest [][20], int count);


/// Converts array of generic type to array Strings
/// \param[in]	scr		Incoming array
/// \param[out]	scr		Outgoing array
/// \param[in]	scr		Number of elements in array
template <typename Tin> 
void		convert_array(Tin src[], String dest[], int count)
{ 
	for (int i = 0; i < count; i++) {
		dest[i] = String(src[i]); 
	}
}


/// Converts array of generic type to array of different provided type
/// \param[in]	scr		Incoming array
/// \param[out]	scr		Outgoing array
/// \param[in]	scr		Number of elements in array
template <typename Tin, typename Tout> 
void		convert_array(Tin src [], Tout dest[], int count)
{ 
	String tmps[count]; 
	convert_array(src, tmps, count); 
	convert_array(tmps, dest, count); 
}



#endif // of #ifndef LOOM_TRANSLATOR_h

