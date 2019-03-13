
#ifndef LOOM_DECAGON_5TM_h
#define LOOM_DECAGON_5TM_h

#include "Loom_SDI12_Sensor.h"



class Loom_Decagon5TM : public LoomSDI12Sensor
{

protected:


public:

	Loom_Decagon5TM(	char* 	module_name			= "5TM Decagon",
						char* 	sensor_description 	= "Soil Moisture", 
						uint8_t	num_samples 		= 1 

			  );


	// --- DESTRUCTOR ---
	virtual ~Loom_Decagon5TM();

	// Inherited Methods
	void		print_config();
	void		print_measurements();
	void		calibrate() {};
	void		measure();
	void		package(OSCBundle& bndl, char* suffix="");

private:


};



#endif // of #ifndef LOOM_DECAGON_5TM_h
