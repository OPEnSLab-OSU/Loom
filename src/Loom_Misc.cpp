#include "Loom_Misc.h"

/////////////////////////////////////////////////////////////////////
void print_array(String data [], int len, int format)
{
	#if LOOM_DEBUG == 1
		for (int i = 0; i < len; i++) {
			if (format == 1) { Serial.println(data[i].c_str()); }
			if (format  > 1) { Serial.print(data[i].c_str()); Serial.print(" "); }
			if ((format > 2) && (i%5==0)) { Serial.println(); }
		}
		Serial.println();
	#endif
}

/////////////////////////////////////////////////////////////////////
bool functionRoute(const char* name) 
{ 
	return false; 
}
