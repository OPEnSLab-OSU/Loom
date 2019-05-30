
#ifndef LOOM_MACROS_h
#define LOOM_MACROS_h

// If LOOM_DEBUG was never specified elsewhere, default to enabled
#ifndef LOOM_DEBUG
	#define LOOM_DEBUG 1
#endif




#define ARR_EXPAND1(x) x[0]
#define ARR_EXPAND2(x) ARR_EXPAND1(x), x[1]
#define ARR_EXPAND3(x) ARR_EXPAND2(x), x[2]
#define ARR_EXPAND4(x) ARR_EXPAND3(x), x[3]
#define ARR_EXPAND5(x) ARR_EXPAND4(x), x[4]
#define ARR_EXPAND6(x) ARR_EXPAND5(x), x[5]
#define ARR_EXPAND7(x) ARR_EXPAND6(x), x[6]
#define ARR_EXPAND8(x) ARR_EXPAND7(x), x[7]
#define ARR_EXPAND9(x) ARR_EXPAND8(x), x[8]
#define ARR_EXPAND10(x) ARR_EXPAND9(x), x[9]
#define ARR_EXPAND(x, i) ARR_EXPAND##i(x)




#define VARIADIC_DETAIL_CAT2(a, b) a ## b
#define VARIADIC_DETAIL_CAT(a, b) VARIADIC_DETAIL_CAT2(a, b)

#define VARIADIC_EXPAND(...) \
    int VARIADIC_DETAIL_CAT(libutil_expando, __COUNTER__) [] = { 0, \
      ((__VA_ARGS__), 0)... \
    } \
    /**/


// Functions to print arbitrary number of elements at once

template <typename... Types> 
void LPrint(Types... vars) 
{
	#if LOOM_DEBUG == 1
		VARIADIC_EXPAND(Serial.print(vars));
	#endif
}

template <typename... Types> 
void LPrintln(Types... vars) 
{		
	#if LOOM_DEBUG == 1
		VARIADIC_EXPAND(Serial.print(vars));
		Serial.println();
	#endif
}

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


#endif
