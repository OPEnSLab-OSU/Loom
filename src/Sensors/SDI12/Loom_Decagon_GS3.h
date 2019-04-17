
#ifndef LOOM_DECAGON_GS3_h
#define LOOM_DECAGON_GS3_h

#include "Loom_SDI12_Sensor.h"


// # (LoomSDI12Sensor) | dependencies: [] | conflicts: []
/// Decagon GS3 Soil Moisture sensor module
// #
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

	/// Decagon GS3 module constructor
	///
	/// \param[in]	module_name			String | <"GS3"> | Decagon GS3 manager module name
	/// \param[in]	sensor_description	String | <"GS3 Decagon"> | Debugging display description of module
	/// \param[in]	num_samples			Set(Int) | <8> | {1, 2, 4, 8, 16} | How many samples to take and average
	Loom_DecagonGS3(	
			char*		module_name			= "GS3 Decagon",
			char*		sensor_description	= "Soil Moisture",
			uint8_t		num_samples			= 1
		);


	// --- DESTRUCTOR ---
	virtual ~Loom_DecagonGS3();

	// Inherited Methods
	void		print_config() override;
	void		print_measurements() override;
	void		calibrate() override;
	void		measure() override;
	void		package(OSCBundle& bndl, char* suffix="") override;

private:


};



#endif // of #ifndef LOOM_DECAGON_GS3_h
