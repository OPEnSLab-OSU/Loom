// ================================================================ 
// ===                         FORMAT                           === 
// ================================================================

// This files is the recommended place for placing wrappers for 
// functions to be called by the hub scripts.
// That is, the script calls these functions (as they have a known 
// type signature) which in turn translate those arguments and
// return to properly interface with existing or new functions that
// cannot be directly called by the script


// ================================================================ 
// ===                 WRAPPER TYPE SIGNATURE                   === 
// ================================================================

// Functions that are called by the script parser are expected to 
// have the following type signature:

// 		float addition(float x, float y)


// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================ 



// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================




// ================================================================ 
// ===           REGISTER ACCESS FUNCTION PROTOTYPES            === 
// ================================================================
float load_R(int i); 				// returns the value in the ith register of R bank
float load_S(int i); 				// returns the value in the ith register of S bank
float load_T(int i); 				// returns the value in the ith register of T bank
void  store_R(int i, float val); 	// sets the value in the ith register of R bank to be val
void  store_S(int i, float val); 	// sets the value in the ith register of S bank to be val
void  store_T(int i, float val); 	// sets the value in the ith register of T bank to be val


// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================



// ================================================================
// ===                        FUNCTIONS                         ===
// ================================================================

