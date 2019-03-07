
#ifndef LOOM_MACROS_h
#define LOOM_MACROS_h


#ifndef LOOM_DEBUG
	#define LOOM_DEBUG 1
#endif

// Print or println up to 5 items at once
#define Print(X)            (LOOM_DEBUG==0) ? :  Serial.print(X)
#define Println(X)          (LOOM_DEBUG==0) ? :  Serial.println(X)
#define Print2(X,Y)         Print(X); Print(Y)
#define Println2(X,Y)       Print(X); Println(Y)
#define Print3(X,Y,Z)       Print(X); Print(Y); Print(Z)
#define Println3(X,Y,Z)     Print(X); Print(Y); Println(Z)
#define Print4(W,X,Y,Z)     Print(W); Print(X); Print(Y); Print(Z)
#define Println4(W,X,Y,Z)   Print(W); Print(X); Print(Y); Println(Z)
#define Print5(V,W,X,Y,Z)   Print(V); Print(W); Print(X); Print(Y); Print(Z)
#define Println5(V,W,X,Y,Z) Print(V); Print(W); Print(X); Print(Y); Println(Z)

// Print Hexadeximal Numbers
#define Print_Hex(X)      (LOOM_DEBUG==0) ? :  Serial.print(X, HEX)
#define Println_Hex(X)    (LOOM_DEBUG==0) ? :  Serial.println(X, HEX)

// Priint Hexadecimal Numbers in form: DEC (0xHEX)
#define Print_Dec_Hex(X)      (LOOM_DEBUG==0) ? :  Serial.print(X); Serial.print(" (0x"); Serial.print(X, HEX); Serial.print(")")
#define Println_Dec_Hex(X)    (LOOM_DEBUG==0) ? :  Serial.print(X); Serial.print(" (0x"); Serial.print(X, HEX); Serial.println(")")


#endif
