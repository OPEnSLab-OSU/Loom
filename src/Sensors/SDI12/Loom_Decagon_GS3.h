
#ifndef LOOM_DECAGON_GS3_h
#define LOOM_DECAGON_GS3_h

#include "Loom_SDI12_Sensor.h"



class Loom_DecagonGS3 : public LoomSDI12Sensor
{

protected:

	/// Measured dielectric permativity
	float		dielec_perm;
	/// Measured temperature
	float		temp;
	/// Measure electrical conductivity
	float		elec_cond;

public:

	Loom_DecagonGS3(	
			char*		module_name			= DECAGON_GS3_Module_Name, // "GS3 Decagon",
			char*		sensor_description	= DECAGON_GS3_Sensor_Description, // "Soil Moisture", 
			uint8_t		num_samples			= DECAGON_GS3_Num_Samples // 1 
		);


	// --- DESTRUCTOR ---
	virtual ~Loom_DecagonGS3();

	// Inherited Methods
	void		print_config();
	void		print_measurements();
	void		calibrate() {};
	void		measure();
	void		package(OSCBundle& bndl, char* suffix="");

private:


};



#endif // of #ifndef LOOM_DECAGON_GS3_h
