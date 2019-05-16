
#ifndef LOOM_SENSOR_h
#define LOOM_SENSOR_h


#include "Loom_Module.h"


/// Abstract root of Loom sensor modules hierarchy
class LoomSensor : public LoomModule
{

protected:

	/// How many samples to take
	uint8_t		num_samples;

	// struct or individual members of data?
	// might prefer individual, using arrays for similar elements (e.g. different axes)
	// not really a reason for struct unless it is being passes around

public:

	LoomSensor(	
			const char*		module_name		= "Sensor",
 			uint8_t			num_samples		= 1 
		  );


	// --- DESTRUCTOR ---
	virtual ~LoomSensor();

	// Inherited Methods
	virtual void	print_config() override;
	virtual void	print_measurements() = 0; 
	virtual void	calibrate() {};
	virtual void	measure() = 0;
	virtual void	package(OSCBundle& bndl, char* suffix="") = 0;
	virtual bool	message_route(OSCMessage& msg, int address_offset) {};
	virtual void 	package(JsonObject json) = 0;


	/// Set the number of samples to take
	/// \param[in]	n	Number of samples to take
	void			set_num_samples(uint8_t n);
	/// Get the number of samples sensor is set to take
	/// \return		Number of samples
	uint8_t			get_num_samples();

private:


};



#endif
