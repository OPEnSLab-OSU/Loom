
#ifndef LOOM_DECAGON_GS3_h
#define LOOM_DECAGON_GS3_h

#include "Loom_SDI12_Sensor.h"



class Loom_DecagonGS3 : public LoomSDI12Sensor
{

protected:


	float dielec_perm;
	float temp;
	float elec_cond;

public:

	Loom_DecagonGS3(	char* 	module_name			= "GS3 Decagon",
						char* 	sensor_description 	= "Soil Moisture", 
						uint8_t	num_samples 		= 1 

			  );


	// --- DESTRUCTOR ---
	virtual ~Loom_DecagonGS3();

	void print_config();
	void print_measurements();
	void calibrate() {};
	void measure();
	void package(OSCBundle& bndl, char* suffix="");

private:


};



#endif // of #ifndef LOOM_DECAGON_GS3_h
