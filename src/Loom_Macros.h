
#ifndef LOOM_MACROS_h
#define LOOM_MACROS_h

// If LOOM_DEBUG was never specified elsewhere, default to enabled
#ifndef LOOM_DEBUG
	#define LOOM_DEBUG 1
#endif



template <typename... Types> 
void LPrint(Types... vars) 
{
	#if LOOM_DEBUG == 1
		using expand_type = int[];
		expand_type{ (Serial.print(vars), 0)... };
	#endif
}

template <typename... Types> 
void LPrintln(Types... vars) 
{		
	#if LOOM_DEBUG == 1
		using expand_type = int[];
		expand_type{ (Serial.print(vars), 0)... };
		Serial.println();
	#endif
}

template <typename... Types> 
void LPrintlnAll(Types... vars) 
{
	#if LOOM_DEBUG == 1
		using expand_type = int[];
		expand_type{ (Serial.println(vars), 0)... };
		Serial.println();
	#endif
}

// Macros for print or println up to 5 items at once

/// LPrint X to Serial if LOOM_DEBUG enabled, no newline
// #define LPrint(X)            (LOOM_DEBUG==0) ? :  Serial.print(X)
// /// LPrint X to Serial if LOOM_DEBUG enabled, newline added
// #define LPrintln(X)          (LOOM_DEBUG==0) ? :  Serial.println(X)
// /// LPrint X,Y to Serial if LOOM_DEBUG enabled, no newline
// #define LPrint(X,Y)         LPrint(X); LPrint(Y)
// /// LPrint X,Y to Serial if LOOM_DEBUG enabled, newline added
// #define LPrintln(X,Y)       LPrint(X); LPrintln(Y)
// /// LPrint X,Y,Z to Serial if LOOM_DEBUG enabled, no newline
// #define LPrint(X,Y,Z)       LPrint(X); LPrint(Y); LPrint(Z)
// /// LPrint X,Y,Z to Serial if LOOM_DEBUG enabled, newline added
// #define LPrintln(X,Y,Z)     LPrint(X); LPrint(Y); LPrintln(Z)
// /// LPrint W,X,Y,Z to Serial if LOOM_DEBUG enabled, no newline
// #define LPrint(W,X,Y,Z)     LPrint(W); LPrint(X); LPrint(Y); LPrint(Z)
// /// LPrint W,X,Y,Z to Serial if LOOM_DEBUG enabled, newline added
// #define LPrintln(W,X,Y,Z)   LPrint(W); LPrint(X); LPrint(Y); LPrintln(Z)
// /// LPrint V,W,X,Y,Z to Serial if LOOM_DEBUG enabled, no newline
// #define LPrint(V,W,X,Y,Z)   LPrint(V); LPrint(W); LPrint(X); LPrint(Y); LPrint(Z)
// /// LPrint V,W,X,Y,Z to Serial if LOOM_DEBUG enabled, newline added
// #define LPrintln(V,W,X,Y,Z) LPrint(V); LPrint(W); LPrint(X); LPrint(Y); LPrintln(Z)

/// LPrint Hexadeximal number to Serial if LOOM_DEBUG enabled, no newline
#define LPrint_Hex(X)      (LOOM_DEBUG==0) ? :  Serial.print(X, HEX)
/// LPrint Hexadeximal number to Serial if LOOM_DEBUG enabled, newline added
#define LPrintln_Hex(X)    (LOOM_DEBUG==0) ? :  Serial.println(X, HEX)

/// LPrint Hexadeximal number to Serial in form: DEC (0xHEX) if LOOM_DEBUG enabled, newline added
#define LPrint_Dec_Hex(X)      (LOOM_DEBUG==0) ? :  Serial.print(X); Serial.print(" (0x"); Serial.print(X, HEX); Serial.print(")")
/// LPrint Hexadeximal number to Serial in form: DEC (0xHEX) if LOOM_DEBUG enabled, newline added
#define LPrintln_Dec_Hex(X)    (LOOM_DEBUG==0) ? :  Serial.print(X); Serial.print(" (0x"); Serial.print(X, HEX); Serial.println(")")


#endif
