
#ifndef LOOM_DECAGON_5TM_h
#define LOOM_DECAGON_5TM_h

#include "Loom_SDI12_Sensor.h"


// ### (LoomSDI12Sensor) | dependencies: [] | conflicts: []
/// Decagon 5TM soil moisture sensor module
// ###
class Loom_Decagon5TM : public LoomSDI12Sensor
{

protected:


public:

	/// Decagon 5TM module constructor
	///
	/// \param[in]	module_name			String | <"5TM"> | null | Decagon 5TM manager module name
	/// \param[in]	num_samples			Set(Int) | <8> | {1, 2, 4, 8, 16} | How many samples to take and average
	Loom_Decagon5TM(
			const char*		module_name		= "5TM Decagon",
			uint8_t			num_samples		= 1
		);

	Loom_Decagon5TM(JsonVariant p);


	// --- DESTRUCTOR ---
	virtual ~Loom_Decagon5TM();

	// Inherited Methods
	void		print_config() override;
	void		print_measurements() override;
	// void		calibrate() override;
	void		measure() override;
	void		package(OSCBundle& bndl, char* suffix="") override;

private:


};



#endif // of #ifndef LOOM_DECAGON_5TM_h
