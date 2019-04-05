
#ifndef LOOM_DECAGON_5TM_h
#define LOOM_DECAGON_5TM_h

#include "Loom_SDI12_Sensor.h"


/// Decagon 5TM soil moisture sensor module
class Loom_Decagon5TM : public LoomSDI12Sensor
{

protected:


public:

	Loom_Decagon5TM(	
			char*		module_name			=  "5TM Decagon",// DECAGON_5TM_Module_Name,
			char*		sensor_description	=  "Soil Moisture", // DECAGON_5TM_Sensor_Description,
			uint8_t		num_samples			= 1 // DECAGON_5TM_Num_Samples 
		);


	// --- DESTRUCTOR ---
	virtual ~Loom_Decagon5TM();

	// Inherited Methods
	void		print_config() override;
	void		print_measurements() override;
	void		calibrate() override;
	void		measure() override;
	void		package(OSCBundle& bndl, char* suffix="") override;

private:


};



#endif // of #ifndef LOOM_DECAGON_5TM_h
