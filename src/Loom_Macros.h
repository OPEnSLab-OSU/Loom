
#ifndef LOOM_MACROS_h
#define LOOM_MACROS_h

// If LOOM_DEBUG was never specified elsewhere, default to enabled
#ifndef LOOM_DEBUG
	#define LOOM_DEBUG 1
#endif


// Macros for print or println up to 5 items at once

/// Print X to Serial if LOOM_DEBUG enabled, no newline
#define Print(X)            (LOOM_DEBUG==0) ? :  Serial.print(X)
/// Print X to Serial if LOOM_DEBUG enabled, newline added
#define Println(X)          (LOOM_DEBUG==0) ? :  Serial.println(X)
/// Print X,Y to Serial if LOOM_DEBUG enabled, no newline
#define Print2(X,Y)         Print(X); Print(Y)
/// Print X,Y to Serial if LOOM_DEBUG enabled, newline added
#define Println2(X,Y)       Print(X); Println(Y)
/// Print X,Y,Z to Serial if LOOM_DEBUG enabled, no newline
#define Print3(X,Y,Z)       Print(X); Print(Y); Print(Z)
/// Print X,Y,Z to Serial if LOOM_DEBUG enabled, newline added
#define Println3(X,Y,Z)     Print(X); Print(Y); Println(Z)
/// Print W,X,Y,Z to Serial if LOOM_DEBUG enabled, no newline
#define Print4(W,X,Y,Z)     Print(W); Print(X); Print(Y); Print(Z)
/// Print W,X,Y,Z to Serial if LOOM_DEBUG enabled, newline added
#define Println4(W,X,Y,Z)   Print(W); Print(X); Print(Y); Println(Z)
/// Print V,W,X,Y,Z to Serial if LOOM_DEBUG enabled, no newline
#define Print5(V,W,X,Y,Z)   Print(V); Print(W); Print(X); Print(Y); Print(Z)
/// Print V,W,X,Y,Z to Serial if LOOM_DEBUG enabled, newline added
#define Println5(V,W,X,Y,Z) Print(V); Print(W); Print(X); Print(Y); Println(Z)

/// Print Hexadeximal number to Serial if LOOM_DEBUG enabled, no newline
#define Print_Hex(X)      (LOOM_DEBUG==0) ? :  Serial.print(X, HEX)
/// Print Hexadeximal number to Serial if LOOM_DEBUG enabled, newline added
#define Println_Hex(X)    (LOOM_DEBUG==0) ? :  Serial.println(X, HEX)

/// Print Hexadeximal number to Serial in form: DEC (0xHEX) if LOOM_DEBUG enabled, newline added
#define Print_Dec_Hex(X)      (LOOM_DEBUG==0) ? :  Serial.print(X); Serial.print(" (0x"); Serial.print(X, HEX); Serial.print(")")
/// Print Hexadeximal number to Serial in form: DEC (0xHEX) if LOOM_DEBUG enabled, newline added
#define Println_Dec_Hex(X)    (LOOM_DEBUG==0) ? :  Serial.print(X); Serial.print(" (0x"); Serial.print(X, HEX); Serial.println(")")


#endif
