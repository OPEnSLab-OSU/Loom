///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_Macros.h
/// @brief		File contains general purpose Loom macros
/// @details	
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#pragma once

// If LOOM_DEBUG was never specified elsewhere, default to enabled
#ifndef LOOM_DEBUG
	#define LOOM_DEBUG 1	///< 1 to enable debug prints (and all LPrint(), LPrintln() calls), 0 to disable
#endif


/// \cond DO_NOT_DOCUMENT
#define EXPAND_ARRAY1(x) x[0]
#define EXPAND_ARRAY2(x)  EXPAND_ARRAY1(x),  x[1]
#define EXPAND_ARRAY3(x)  EXPAND_ARRAY2(x),  x[2]
#define EXPAND_ARRAY4(x)  EXPAND_ARRAY3(x),  x[3]
#define EXPAND_ARRAY5(x)  EXPAND_ARRAY4(x),  x[4]
#define EXPAND_ARRAY6(x)  EXPAND_ARRAY5(x),  x[5]
#define EXPAND_ARRAY7(x)  EXPAND_ARRAY6(x),  x[6]
#define EXPAND_ARRAY8(x)  EXPAND_ARRAY7(x),  x[7]
#define EXPAND_ARRAY9(x)  EXPAND_ARRAY8(x),  x[8]
#define EXPAND_ARRAY10(x) EXPAND_ARRAY9(x),  x[9]
#define EXPAND_ARRAY11(x) EXPAND_ARRAY10(x), x[10]
#define EXPAND_ARRAY12(x) EXPAND_ARRAY11(x), x[11]
#define EXPAND_ARRAY13(x) EXPAND_ARRAY12(x), x[12]
#define EXPAND_ARRAY14(x) EXPAND_ARRAY13(x), x[13]
#define EXPAND_ARRAY15(x) EXPAND_ARRAY14(x), x[14]
/// \endcond

/// Expand an array into individual arguments.
/// Use by module constructors, expanding the JsonArray into arguments to 
/// Call regular constructor with.
/// @param[in]	x	Array to expand
/// @param[in]	i	Number of elements to expand.
/// 				Do not use with a value larger than the size of the array
#define EXPAND_ARRAY(x, i) EXPAND_ARRAY##i(x)



/// \cond DO_NOT_DOCUMENT
/// Macros to expand variadics.
/// For use in LPrint, LPrintln, and LPrintlnAll functions
#define VARIADIC_DETAIL_CAT2(a, b) a ## b
#define VARIADIC_DETAIL_CAT(a, b) VARIADIC_DETAIL_CAT2(a, b)

#define VARIADIC_EXPAND(...) \
    int VARIADIC_DETAIL_CAT(libutil_expando, __COUNTER__) [] = { 0, \
      ((__VA_ARGS__), 0)... \
    } \
    /**/
/// \endcond


/// Print variable number of arguments (no newline)
template <typename... Types> 
void LPrint(Types... vars) 
{
	#if LOOM_DEBUG == 1
		VARIADIC_EXPAND(Serial.print(vars));
	#endif
}

/// Print variable number of arguments 
/// with newline after last element
template <typename... Types> 
void LPrintln(Types... vars) 
{		
	#if LOOM_DEBUG == 1
		VARIADIC_EXPAND(Serial.print(vars));
		Serial.println();
	#endif
}

/// Print variable number of arguments 
/// with newline after each element
template <typename... Types> 
void LPrintlnAll(Types... vars) 
{
	#if LOOM_DEBUG == 1
		VARIADIC_EXPAND(Serial.println(vars));
		Serial.println();
	#endif
}

/// LPrint Hexadeximal number to Serial if LOOM_DEBUG enabled, no newline
#define LPrint_Hex(X)      (LOOM_DEBUG==0) ? :  Serial.print(X, HEX)
/// LPrint Hexadeximal number to Serial if LOOM_DEBUG enabled, newline added
#define LPrintln_Hex(X)    (LOOM_DEBUG==0) ? :  Serial.println(X, HEX)
/// LPrint Hexadeximal number to Serial in form: DEC (0xHEX) if LOOM_DEBUG enabled, newline added
#define LPrint_Dec_Hex(X)      (LOOM_DEBUG==0) ? :  Serial.print(X); Serial.print(" (0x"); Serial.print(X, HEX); Serial.print(")")
/// LPrint Hexadeximal number to Serial in form: DEC (0xHEX) if LOOM_DEBUG enabled, newline added
#define LPrintln_Dec_Hex(X)    (LOOM_DEBUG==0) ? :  Serial.print(X); Serial.print(" (0x"); Serial.print(X, HEX); Serial.println(")")








