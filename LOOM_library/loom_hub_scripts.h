// ================================================================ 
// ===                        LIBRARIES                         === 
// ================================================================
#include <math.h>


// ================================================================ 
// ===                       DEFINITIONS                        === 
// ================================================================
typedef float (*retFuncPtr)(float,float);


// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================ 


// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================

// Some struct used to be saved to flash


float stack[50];    
int   stackPtr = 0;
char  data_type;
char  buf[20];
bool  takingElseBranch = false;

// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
retFuncPtr strToFunc(char * str);
float msgGetLiteral(OSCMessage* msg, int pos);
void  parseProgram(OSCMessage* msg);


// ================================================================ 
// ===                      MINI FUNCTIONS                      === 
// ================================================================

// Single value math
float inc(float x, float n) { return x+1; } 
float dec(float x, float n) { return x-1; }
float square(float x, float n) { return x * x; }

// Math
float addition(float x, float y) { return x + y; }
float subtract(float x, float y) { return x - y; }
float multiply(float x, float y) { return x * y; }
float divide(  float x, float y) { return x / y; }
float power(   float x, float y) { return pow(x, y); }

// Comparisons
float equal(        float x, float y) { return x == y; }
float notEqual(     float x, float y) { return x != y; }
float lessThan(     float x, float y) { return x <  y; }
float greaterThan(  float x, float y) { return x >  y; }
float lessThanEq(   float x, float y) { return x <= y; }
float greaterThanEq(float x, float y) { return x >= y; }

// Logic gates
float logical_not( float a, float n) { return !(bool)a; } 
float logical_or(  float a, float b) { return (bool)a || (bool)b; } 
float logical_and( float a, float b) { return (bool)a && (bool)b; } 
float logical_nor( float a, float b) { return !((bool)a || (bool)b); } 
float logical_nand(float a, float b) { return !((bool)a && (bool)b); }
float logical_xor( float a, float b) { return ( ((bool)a && !(bool)b) || (!(bool)a && (bool)b) ); } 

// Printing / Void fuctions
float ifFuncEval(float y, float z) { Serial.println("Evaluated True");  return 0; }
float elFuncEval(float y, float z) { Serial.println("Evaluated False"); return 0; }
float termFunc(  float y, float z) { Serial.println("Function chain terminated"); return 0; }
float errorFunc( float y, float z) { Serial.println("Function pointer was null"); return 0; }
float printVal(  float y, float z) { Serial.print("Value: "); Serial.println(y);  return 0; }


// Wrapper for any 'float(*ptr)(float,float)' functions
float wrapper(float x, float y, float (*fPtr)(float,float)) {
	return fPtr(x, y);
}


// ================================================================
// ===                          SETUP                           ===
// ================================================================


void setup_hub_scripts()
{
	// Probably will just entail reading the scripts (and maybe registers)
	// from flash memory

	// Clear out 'registers'
}


// ================================================================
// ===                        FUNCTIONS                         ===
// ================================================================


void parseProgram(OSCMessage* msg)
{
	for (int i = 0; i < msg.size(); i++) {

		data_type = msg.getType(i);
		Serial.print("Data Type: "); Serial.print(data_type); Serial.print(" : ");

		switch(data_type) {
			// Number or boolean literals
			case 'd': case 'f': case 'i': case 'b':
				Serial.println(     msgGetLiteral(&msg, i));
				stack[stackPtr++] = msgGetLiteral(&msg, i);
				break;

			// Commands and Functions and If Statements
			case 's':
				msg.getString(i, buf, 20);
				Serial.println(buf);

				// Pop one value
				if ( strcmp(buf, "pop") == 0 ) {  
					stackPtr--;  break;  
				}

				// Unary Math operators
				if ( (strcmp(buf, "++") == 0) || (strcmp(buf, "--") == 0) || (strcmp(buf, "sqr") == 0) ) {  
					stack[stackPtr-1] = strToFunc(buf)(stack[stackPtr-1], 0.0);  break;  
				}

				// Math operators
				if ( (strcmp(buf,"+")==0) || (strcmp(buf,"-")==0) || (strcmp(buf,"*")==0) || (strcmp(buf,"/")==0) || (strcmp(buf,"^")==0)) {
					stack[stackPtr-2] = strToFunc(buf)(stack[stackPtr-2], stack[stackPtr-1]);  stackPtr--;  break;
				}

				// Reading from External Variables 
				if (strncmp(buf, "read_", 5) == 0) {  
					stack[stackPtr++] = reg[ buf[5]-114 ][ (int)strtol(buf+6,NULL,10) ];  break;  
				}
				
				// Storing to External Variables
				if (strncmp(buf, "store_", 6) == 0) {  
					reg[ buf[6]-114 ][ (int)strtol(buf+7,NULL,10) ] = stack[stackPtr-1];  break;  
				}

				// If Statements
				if ( strcmp(buf, "if") == 0 ) {      
					// Grab the condition value from the stack
					// If taking the 'else' branch, skip the 'then' branch
					if (!stack[--stackPtr]) {
						Serial.println("Taking Else branch");
						takingElseBranch = true;
						msg.getString(++i, buf, 20);
						while ( (strcmp(buf, "else") != 0) && (strcmp(buf, "endif") != 0) ) {
							msg.getString(++i, buf, 20);
						}
					} else {
						Serial.println("Taking Then branch");
					}
				}

				if ( strcmp(buf, "else") == 0 ) {  
					// If taking the else branch, just continue
    				// Otherwise skip to 'endif'
					if (!takingElseBranch) {
						msg.getString(++i, buf, 20);
						while( strcmp(buf, "endif") != 0 ) {
							msg.getString(++i, buf, 20);
						}
					}
					takingElseBranch = false;
				}

				if ( strcmp(buf, "endif") == 0) {
					// probably don't need to do anything here
					// takingElseBranch = false;
				}

				break;

			default:
				Serial.print("Parser error, invalid program detected at symbol #: ");
				Serial.println(i);
				break;
		}

		Serial.print("Stack ("); Serial.print(i); Serial.print("):");
		for (int j = 0; j < stackPtr; j++) {
			Serial.print("  "); Serial.print(stack[j]);
		} 
		Serial.println();

	}

	Serial.print("Result: ");
	Serial.println(stack[stackPtr-1]);
	Serial.println();

	Serial.print("R: ");
	for (int i = 0; i < 10; i++) {
		Serial.print(i); Serial.print(": "); Serial.print(reg[0][i]); Serial.print(", "); 
	}
	Serial.print("S: ");
	for (int i = 0; i < 10; i++) {
		Serial.print(i); Serial.print(": "); Serial.print(reg[1][i]); Serial.print(", "); 
	}
	Serial.print("T: ");
	for (int i = 0; i < 10; i++) {
		Serial.print(i); Serial.print(": "); Serial.print(reg[2][i]); Serial.print(", "); 
	}
}




// --- MESSAGE GET LITERAL ---
// 
// This function attempts to grab a number or boolean
// value from a message and return it as a float.
// It works if the value is a double, float, int, or bool
//
float msgGetLiteral(OSCMessage* msg, int pos) 
{
	switch(msg->getType(pos)) {
		case 'd': return (float)msg->getDouble(pos);
		case 'f': return (float)msg->getFloat(pos); 
		case 'i': return (float)msg->getInt(pos); 			
		case 'b': return (float)msg->getBoolean(pos);
		default:  return (float)-1.0;
	}
}



// --- STRING TO FUNCTION ---
//
// This function takes a string that matches or
// represents a function, and returns a pointer of 
// the corresponding function
//
retFuncPtr strToFunc(char * str) {
	if ( (strcmp(str, "inc") == 0)    || (strcmp(str, "++")  == 0) ) return inc;
	if ( (strcmp(str, "dec") == 0)    || (strcmp(str, "--")  == 0) ) return dec;
	if ( (strcmp(str, "square") == 0) || (strcmp(str, "sqr") == 0) ) return square;
	
	if ( (strcmp(str, "addition") == 0)      || (strcmp(str, "+") == 0 ) ) return addition;
	if ( (strcmp(str, "subtract") == 0)      || (strcmp(str, "-") == 0 ) ) return subtract;
	if ( (strcmp(str, "multiply") == 0)      || (strcmp(str, "*") == 0 ) ) return multiply;
	if ( (strcmp(str, "divide") == 0)        || (strcmp(str, "/") == 0 ) ) return divide;
	if ( (strcmp(str, "power") == 0)         || (strcmp(str, "^") == 0 ) ) return power;	

	if ( (strcmp(str, "equal") == 0)         || (strcmp(str, "==") == 0) ) return equal;
	if ( (strcmp(str, "notEqual") == 0)      || (strcmp(str, "!=") == 0) ) return notEqual;
	if ( (strcmp(str, "lessThan") == 0)      || (strcmp(str, "<")  == 0) ) return lessThan;
	if ( (strcmp(str, "greaterThan") == 0)   || (strcmp(str, ">")  == 0) ) return greaterThan;
	if ( (strcmp(str, "lessThanEq") == 0)    || (strcmp(str, "<=") == 0) ) return lessThanEq;
	if ( (strcmp(str, "greaterThanEq") == 0) || (strcmp(str, ">=") == 0) ) return greaterThanEq;
	
	if ( (strcmp(str, "logical_not")  == 0)  || (strcmp(str, "!")  == 0) ) return logical_not;
	if ( (strcmp(str, "logical_or")   == 0)  || (strcmp(str, "||") == 0) ) return logical_or;
	if ( (strcmp(str, "logical_and")  == 0)  || (strcmp(str, "&&") == 0) ) return logical_and;
	if ( (strcmp(str, "logical_nor")  == 0)  || (strcmp(str, "!|") == 0) ) return logical_nor;
	if ( (strcmp(str, "logical_nand") == 0)  || (strcmp(str, "!&") == 0) ) return logical_nand;
	if ( (strcmp(str, "logical_xor")  == 0)  || (strcmp(str, "e|") == 0) ) return logical_xor;
	
	if ( (strcmp(str, "ifFuncEval") == 0) ) return ifFuncEval;
	if ( (strcmp(str, "elFuncEval") == 0) ) return elFuncEval;
	if ( (strcmp(str, "termFunc")   == 0) ) return termFunc;
	if ( (strcmp(str, "errorFunc")  == 0) ) return errorFunc;
	if ( (strcmp(str, "printVal")   == 0) ) return printVal;

	return NULL;
}