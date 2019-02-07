
#ifndef LOOM_FONA_h
#define LOOM_FONA_h

#include "Loom_InternetPlat.h"

#include "Adafruit_FONA.h"	// This is currently the default Adafruit FONA library
							// but could be changed to the more refined and featured
							// Botletics extension of the library 
							// (https://github.com/botletics/SIM7000-LTE-Shield)


#define FONA_RX  1
#define FONA_TX  2
#define FONA_RST A0		// Pretty arbitrary, think it justs needs to be an availalble digital out pin

// #define APN "wholesale" // For use with Ting SIM card



class Loom_Fona : public LoomInternetPlat
{

protected:

	Adafruit_FONA* fona;
	HardwareSerial* fonaSerial;


	char* APN;
	
public:

	// --- CONSTRUCTOR ---
	Loom_Fona(	char* 	module_name		= "Fona",

				char* 	apn				= "wholesale"

					);

	// --- DESTRUCTOR ---
	~Loom_Fona();

	// --- PUBLIC METHODS ---

	void print_config();
	void print_state();



	bool connect();
	bool is_connected();
	uint32_t get_time() {}


	void fona_test_loop();


	// Should this be external?
	// bool log_to_pushingbox(OSCBundle* bndl);

private:

	void     printMenu();
	uint8_t  readline(char *buff, uint8_t maxbuff, uint16_t timeout = 0);
	void     flushSerial();
	char     readBlocking();
	uint16_t readnumber();

};



#endif

