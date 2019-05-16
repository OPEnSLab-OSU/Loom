
#ifndef LOOM_MACROS_h
#define LOOM_MACROS_h

// If LOOM_DEBUG was never specified elsewhere, default to enabled
#ifndef LOOM_DEBUG
	#define LOOM_DEBUG 1
#endif



// void func();// {} // termination version

// template<typename Arg1, typename... Args>
// void func(const Arg1 arg1, const Args... args)
// {
//     Serial.println( arg1 );
//     func(args...); // note: arg1 does not appear here!
// }




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
