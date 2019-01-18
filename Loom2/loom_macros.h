
#ifndef LOOM_MACROS_h
#define LOOM_MACROS_h



#define LOOM_DEBUG 1


// Print or println up to 5 items at once
#define LOOM_DEBUG_Print(X)            (LOOM_DEBUG==0) ? :  Serial.print(X)
#define LOOM_DEBUG_Println(X)          (LOOM_DEBUG==0) ? :  Serial.println(X)
#define LOOM_DEBUG_Print2(X,Y)         LOOM_DEBUG_Print(X); LOOM_DEBUG_Print(Y)
#define LOOM_DEBUG_Println2(X,Y)       LOOM_DEBUG_Print(X); LOOM_DEBUG_Println(Y)
#define LOOM_DEBUG_Print3(X,Y,Z)       LOOM_DEBUG_Print(X); LOOM_DEBUG_Print(Y); LOOM_DEBUG_Print(Z)
#define LOOM_DEBUG_Println3(X,Y,Z)     LOOM_DEBUG_Print(X); LOOM_DEBUG_Print(Y); LOOM_DEBUG_Println(Z)
#define LOOM_DEBUG_Print4(W,X,Y,Z)     LOOM_DEBUG_Print(W); LOOM_DEBUG_Print(X); LOOM_DEBUG_Print(Y); LOOM_DEBUG_Print(Z)
#define LOOM_DEBUG_Println4(W,X,Y,Z)   LOOM_DEBUG_Print(W); LOOM_DEBUG_Print(X); LOOM_DEBUG_Print(Y); LOOM_DEBUG_Println(Z)
#define LOOM_DEBUG_Print5(V,W,X,Y,Z)   LOOM_DEBUG_Print(V); LOOM_DEBUG_Print(W); LOOM_DEBUG_Print(X); LOOM_DEBUG_Print(Y); LOOM_DEBUG_Print(Z)
#define LOOM_DEBUG_Println5(V,W,X,Y,Z) LOOM_DEBUG_Print(V); LOOM_DEBUG_Print(W); LOOM_DEBUG_Print(X); LOOM_DEBUG_Print(Y); LOOM_DEBUG_Println(Z)

// Print Hexadeximal Numbers
#define LOOM_DEBUG_Print_Hex(X)      (LOOM_DEBUG==0) ? :  Serial.print(X, HEX)
#define LOOM_DEBUG_Println_Hex(X)    (LOOM_DEBUG==0) ? :  Serial.println(X, HEX)

// Priint Hexadecimal Numbers in form: DEC (0xHEX)
#define LOOM_DEBUG_Print_Dec_Hex(X)      (LOOM_DEBUG==0) ? :  Serial.print(X); Serial.print(" (0x"); Serial.print(X, HEX); Serial.print(")")
#define LOOM_DEBUG_Println_Dec_Hex(X)    (LOOM_DEBUG==0) ? :  Serial.print(X); Serial.print(" (0x"); Serial.print(X, HEX); Serial.println(")")


#endif