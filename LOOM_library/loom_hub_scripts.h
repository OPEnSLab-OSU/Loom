// ================================================================ 
// ===                        LIBRARIES                         === 
// ================================================================
#include <math.h>


// ================================================================ 
// ===                       DEFINITIONS                        === 
// ================================================================
// Used to make function signatures easier to read 
// when returning function pointers
typedef float (*retFuncPtr)(float,float); 

#define max_static_scripts  10
#define max_dynamic_scripts 10
#define max_script_len      20

// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================ 

// Will be saved to save script to flash

// Do I even need to convert to strings...? probably, if only because the OSCmessage arrays are pointers
struct config_dynamic_scripts_t {
	int num_dynamic_scripts;
	char* dynamic_scripts[5][max_script_len]; // currently up to 5 scripts with as many as 20 commands (arbitrary numbers right now)
};


// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================

struct config_dynamic_scripts_t config_dynamic_scripts;

int num_static_scripts;
int num_dynamic_scripts;

// Scripts that are preloaded and are saved in program memory not flash
OSCMessage* static_msg_scripts[max_static_scripts];
// Scripts that are deleted between restarts unless saved to flash
OSCMessage* dynamic_msg_scripts[max_dynamic_scripts];  

// Used by parser
float stack[50];    
int   stackPtr = 0;
char  data_type;
char  buf[20];
bool  takingElseBranch = false;

// Data registers that persist outside of parser
float reg[3][10];


// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================

// Used for running scripts
void run_all_scripts();
void run_all_static_scripts();
void run_all_dynamic_scripts();
int  get_script_len(String script[]);

// Message router functions
void message_to_script(OSCMessage &msg);
void delete_script(OSCMessage &msg);
void save_dynamic_scripts(OSCMessage &msg);


// Parser and aux functions
void  parseScript(OSCMessage* msg);
void  parseScript(OSCBundle* bndl);
float msgGetLiteral(OSCMessage* msg, int pos);
void  print_registers();
retFuncPtr strToFunc(char * str);


// ================================================================
// ===                    PRELOADED SCRIPTS                     ===
// ================================================================

// This function is where you define any scripts that are loaded to
// the device without needing to be saved to flash (i.e. static scripts)
// Use these for scripts that more or less permanently used on the 
// device until reflashing
void preload_scripts() 
{
	// static_msg_scripts[0] = new OSCMessage("/test");
	// static_msg_scripts[0]->add(10.0).add(300.0).add("blink_ex");
	// num_static_scripts++;

	// static_msg_scripts[1] = new OSCMessage("/test2");
	// static_msg_scripts[1]->add(4.0).add(1000.0).add("blink_ex");
	// num_static_scripts++;

}


// ================================================================ 
// ===                      MINI FUNCTIONS                      === 
// ================================================================

// Returns the value in the rth register of the R (b=0), S (b=1), or T (b=2) bank
float load_reg(int b, int r) { return reg[b][r]; }
// Sets the value in the rth register of the R (b=0), S (b=1), or T (b=2) bank to be val
void  store_reg(int b, int r, float val) { reg[b][r] = val; }


// Single value math
float inc(float x, float n) { return x+1; } 
float dec(float x, float n) { return x-1; }
float square(float x, float n) { return x * x; }

// Math
float addition(float x, float y) { return x + y; }
float subtract(float x, float y) { return x - y; }
float multiply(float x, float y) { return x * y; }
float divide(  float x, float y) { return x / y; }
float exp(   float x, float y) { return pow(x, y); }

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

// Wrapper for any 'float(*ptr)(float,float)' functions
float wrapper(float x, float y, float (*fPtr)(float,float)) {
	return fPtr(x, y);
}



// ================================================================
// ===                          SETUP                           ===
// ================================================================

void setup_hub_scripts()
{
	LOOM_DEBUG_Println("Setting up hub scripts");

	// Clear out 'registers'
	for (int b = 0; b < 3; b++) {
		for (int r = 0; r < 10; r++) {
			reg[b][r] = 0.0;
		}
	}

	num_static_scripts  = 0;
	num_dynamic_scripts = 0;
	preload_scripts();
	
	
	// Setup dynamic scripts
	// i.e. read in from flash
}


// ================================================================
// ===                        FUNCTIONS                         ===
// ================================================================


// --- RUN ALL SCRIPTS --- 
//
// Runs all scripts,
// static and dynamic
//
void run_all_scripts()
{
	run_all_static_scripts();
	run_all_dynamic_scripts();
}



// --- RUN ALL STATIC SCRIPTS --- 
// 
// Runs all static scripts
//
void run_all_static_scripts()
{
	LOOM_DEBUG_Println("Running static scripts");
	for (int i = 0; i < num_static_scripts; i++) {
		parseScript(static_msg_scripts[i]);

		// STRING ARRAY VERSION
		// OSCBundle scriptBndl;
		// // ‘Not actually a key-value array, but this allows the interpret functionality to be used (essential)
		// convert_key_value_array_to_bundle(static_scripts[i], &scriptBndl, "/addr", get_script_len(static_scripts[i]), SINGLEMSG, 4);
		// print_bundle(&scriptBndl);
		// parseScript(&scriptBndl);
	}
}



// --- RUN ALL DYNAMIC SCRIPTS ---
//  
// Runs all dynamic scripts
//
void run_all_dynamic_scripts()
{
	LOOM_DEBUG_Println("Running dynamic scripts");
	for (int i = 0; i < num_dynamic_scripts; i++) {
		parseScript(dynamic_msg_scripts[i]);
	}
}



// Gets the length of the script in String[] format
// as it may not be the max size of the array
int get_script_len(String script[])
{
	for (int i = 0; i < max_script_len; i++) {
		if (script[i] == "done")  return i;
	}

	return max_script_len;
}



// --- MESSAGE TO SCRIPT ---
//
// Turns a received message into a runnable script
//
// @param msg  The message forwarded from msg_router, contains script 
void message_to_script(OSCMessage &msg)
{
	LOOM_DEBUG_Println("Received Script");
	// OSCBundle tmpBndl;
	// tmpBndl.add(msg);
	// print_bundle(&tmpBndl);


	if (num_dynamic_scripts >= max_dynamic_scripts)
		return;

	char script_name[20], buf[20];

	// Get the script name
	msg.getString(0, script_name, 20);

	for (int i = 0; i < num_dynamic_scripts; i++) {
		dynamic_msg_scripts[i]->getAddress(buf);
		// LOOM_DEBUG_Println4("Name: ", script_name, "\tBuf2: ", buf);
		if ( strcmp(script_name, buf) == 0 ) {
			LOOM_DEBUG_Println("Script already on device");
			return;
		}
	}

	dynamic_msg_scripts[num_dynamic_scripts] = new OSCMessage(script_name);

	for (int i = 1; i < msg.size() && i <= max_script_len; i++) {
		switch (msg.getType(i)) {
 			case 'i': dynamic_msg_scripts[num_dynamic_scripts]->add(msg.getInt(i));		break;
 			case 'f': dynamic_msg_scripts[num_dynamic_scripts]->add(msg.getFloat(i));	break;
 			case 's': char tmp[80];  msg.getString(i, tmp, 80);  dynamic_msg_scripts[num_dynamic_scripts]->add(tmp);  break;
 			default: LOOM_DEBUG_Println("Unsupported data data_type.");
 		}
	}

	num_dynamic_scripts++;
}



// --- DELETE SCRIPT---
//
// Deletes the specified script if it exists
// 
// @param msg  The message forwarded from msg_router, 
// 				contains name of script to delete
void delete_script(OSCMessage &msg)
{
	char script_name[20], buf[20];
	msg.getString(0, script_name, 20);
	LOOM_DEBUG_Println2("Command to delete script: ", script_name);


	int i = 0;
	for (int i = 0; i < num_dynamic_scripts; i++) {
		dynamic_msg_scripts[i]->getAddress(buf);

		if ( strcmp(script_name, buf) == 0) {

			for (int j = i; j < num_dynamic_scripts-1; j++) {
				dynamic_msg_scripts[j] = dynamic_msg_scripts[j+1];
			}	
			dynamic_msg_scripts[--num_dynamic_scripts] = NULL;

			LOOM_DEBUG_Println2("Deleted script: ", script_name);
			return;
		}
	}

	LOOM_DEBUG_Println3("Script: ", script_name, " does not exist");
}



// --- SAVE DYNAMIC SCRIPTS ---
//
// Saves dynamic scripts to flash,
// they are lost upon restart if not saved
// 
// @param msg  Message forward by msg_router, required parameter, not used
//
void save_dynamic_scripts(OSCMessage &msg)
{
	
}

// ================================================================
// ===                      SCRIPT PARSER                       ===
// ================================================================



// --- PARSE SCRIPT ---
//
// Takes a script in the form of an OSC message
// and executes it
//
// @param msg  The encoding of the script
//
void parseScript(OSCMessage* msg)
{
	stackPtr = 0;

	char buf[50];
	msg->getAddress(buf, 0);
	LOOM_DEBUG_Println2("Running Script: ", buf );
	for (int i = 0; i < msg->size(); i++) {

		data_type = msg->getType(i);
		Serial.print("Data Type: "); Serial.print(data_type); Serial.print(" : ");

		switch(data_type) {
			// Number or boolean literals
			case 'd': case 'f': case 'i': case 'b':
				Serial.println(     msgGetLiteral(msg, i));
				stack[stackPtr++] = msgGetLiteral(msg, i);
				break;

			// Commands and Functions and If Statements
			case 's': {
				msg->getString(i, buf, 20);
				Serial.println(buf);

				// Pop one value
				if ( strcmp(buf, "pop") == 0 ) {  
					stackPtr--;  break;  
				}

				// Duplicate the value on the top of the stack
				if ( strcmp(buf, "dup") == 0 ) {  
					stack[stackPtr] = stack[stackPtr-1]; stackPtr++;  break;  
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
				if (strncmp(buf, "load_", 5) == 0) {  
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
						LOOM_DEBUG_Println("Taking Else branch");
						takingElseBranch = true;
						msg->getString(++i, buf, 20);
						while ( (strcmp(buf, "else") != 0) && (strcmp(buf, "endif") != 0) ) {
							msg->getString(++i, buf, 20);
						}
					} else {
						LOOM_DEBUG_Println("Taking Then branch");
					}
					break;
				}

				if ( strcmp(buf, "else") == 0 ) {  
					// If taking the else branch, just continue
    				// Otherwise skip to 'endif'
					if (!takingElseBranch) {
						msg->getString(++i, buf, 20);
						while( strcmp(buf, "endif") != 0 ) {
							msg->getString(++i, buf, 20);
						}
					}
					takingElseBranch = false;
					break;
				}

				if ( strcmp(buf, "endif") == 0) {
					// probably don't need to do anything here
					// takingElseBranch = false;
					break;
				}

				if ( strcmp(buf, "printStack") == 0) {
					#if LOOM_DEBUG == 1
						Serial.println();
						for (int j = 0; j < stackPtr; j++) {
							LOOM_DEBUG_Print2("  ", stack[j]); 
						}
						Serial.println();
					#endif
					break;
				}

				// If none of the above commands matched, try seaching external functions
				float (*fPtr)(float,float) = strToFunc(buf);
				if ( fPtr ) {
					stack[stackPtr-2] = fPtr(stack[stackPtr-2], stack[stackPtr-1]);  stackPtr--;  
					break;
				}

				// No 'break' so if error, fallthrough to 'default' case
			} // of case 's':

			default:
				LOOM_DEBUG_Println2("Parser error, invalid program detected at symbol #: ", i);
				break;
		}


// This is just for debugging purposes for now
		Serial.print("Stack ("); Serial.print(i); Serial.print("):");
		for (int j = 0; j < stackPtr; j++) {
			Serial.print("  "); Serial.print(stack[j]);
		} 
		Serial.println();

	}

	Serial.print("Result: ");
	Serial.println(stack[stackPtr-1]);
	Serial.println();


}

// Overloaded version that takes a bundle instead of a message
// Simply calls the normal parseScript with the message in the bundle
void parseScript(OSCBundle* bndl)
{
	if (bndl->size() != 1) {
		LOOM_DEBUG_Println("Bundle not in the correct format for script parser");
		LOOM_DEBUG_Println("Call with either message or bundle containing single message");
	}
	parseScript(bndl->getOSCMessage(0));
}



// ================================================================
// ===               AUXILIARY PARSER FUNCTIONS                 ===
// ================================================================


// --- MESSAGE GET LITERAL ---
// 
// This function attempts to grab a number or boolean
// value from a message and return it as a float.
// It works if the value is a double, float, int, or bool
//
// @param msg  The message to read from
// @param pos  Which parameter of the message to read
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



// --- PRINT REGISTERS ---
//	
// Prints the contents of the 3 register banks
//
void print_registers()
{
	#if LOOM_DEBUG == 1
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
	#endif
}


// --- STRING TO FUNCTION ---
//
// This function takes a string that matches or
// represents a function, and returns a pointer of 
// the corresponding function
//
// @param str  The string to find the matching function for
// 
retFuncPtr strToFunc(char * str) {
	// Math Unary
	if ( (strcmp(str, "inc") == 0)    || (strcmp(str, "++")  == 0) ) return inc;
	if ( (strcmp(str, "dec") == 0)    || (strcmp(str, "--")  == 0) ) return dec;
	if ( (strcmp(str, "square") == 0) || (strcmp(str, "sqr") == 0) ) return square;
	
	// Math 
	if ( (strcmp(str, "add") == 0)           || (strcmp(str, "+") == 0 ) ) return addition;
	if ( (strcmp(str, "subtract") == 0)      || (strcmp(str, "-") == 0 ) ) return subtract;
	if ( (strcmp(str, "multiply") == 0)      || (strcmp(str, "*") == 0 ) ) return multiply;
	if ( (strcmp(str, "divide") == 0)        || (strcmp(str, "/") == 0 ) ) return divide;
	if ( (strcmp(str, "exp") == 0)           || (strcmp(str, "^") == 0 ) ) return exp;	

	// Comparison
	if ( (strcmp(str, "equal") == 0)         || (strcmp(str, "==") == 0) ) return equal;
	if ( (strcmp(str, "notEqual") == 0)      || (strcmp(str, "!=") == 0) ) return notEqual;
	if ( (strcmp(str, "lessThan") == 0)      || (strcmp(str, "<")  == 0) ) return lessThan;
	if ( (strcmp(str, "greaterThan") == 0)   || (strcmp(str, ">")  == 0) ) return greaterThan;
	if ( (strcmp(str, "lessThanEq") == 0)    || (strcmp(str, "<=") == 0) ) return lessThanEq;
	if ( (strcmp(str, "greaterThanEq") == 0) || (strcmp(str, ">=") == 0) ) return greaterThanEq;
	
	// Logic
	if ( (strcmp(str, "not")  == 0)  || (strcmp(str, "!")  == 0) ) return logical_not;
	if ( (strcmp(str, "or")   == 0)  || (strcmp(str, "||") == 0) ) return logical_or;
	if ( (strcmp(str, "and")  == 0)  || (strcmp(str, "&&") == 0) ) return logical_and;
	if ( (strcmp(str, "nor")  == 0)  || (strcmp(str, "!|") == 0) ) return logical_nor;
	if ( (strcmp(str, "nand") == 0)  || (strcmp(str, "!&") == 0) ) return logical_nand;
	if ( (strcmp(str, "xor")  == 0)  || (strcmp(str, "x|") == 0) ) return logical_xor;

	// Custom functions get called here
	return custom_strToFunc(str);
}



