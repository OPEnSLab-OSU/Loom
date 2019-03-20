
#ifndef LOOM_DIGITAL_h
#define LOOM_DIGITAL_h

#include "Loom_Sensor.h"


#define DIGITAL_COUNT 12

// Available digital pins 5, 6, 9, 10, 11, 12, A0(14), A1(15), A2(16), A3(17), A4(18), A5(19)

class Loom_Digital : public LoomSensor
{

protected:

	/// Array of which pins are enabled
	bool		pin_enabled[DIGITAL_COUNT];

	/// Array of last read digital values
	bool		digital_vals[DIGITAL_COUNT];

public:
	// --- CONSTRUCTOR ---
	Loom_Digital(	
			char*	module_name			= "Digital",	//DIGITAL_MODULE_NAME
			char*	sensor_description	= "Digital Values",

			bool	enable5				= true,
			bool	enable6				= true,
			bool	enable9				= false,
			bool	enable10			= true,
			bool	enable11			= false,
			bool	enable12			= false,

			bool	enableA0			= false,
			bool	enableA1			= false,
			bool	enableA2			= false,
			bool	enableA3			= false,
			bool	enableA4			= false,
			bool	enableA5			= false
		);

	// --- DESTRUCTOR ---
	virtual ~Loom_Digital();

	// Inherited Methods
	void		print_config();
	void		print_measurements();
	void		measure();
	void		package(OSCBundle& bndl, char* suffix="");
	bool		message_route(OSCMessage& msg, int address_offset) {}


	/// Get value on digital pin
	/// \return		True if HIGH, false if LOW
	bool		get_digital_val(uint8_t pin);
	/// Set digital value of pin
	/// \param[in]	pin		Pin to set value of
	/// \param[in]	state	State to set pin to
	void		set_digital_val(uint8_t pin, bool state);

	/// Get if pin is enabled in manager
	/// \return 	Enable state of pin
	bool		get_pin_enabled(uint8_t pin);
	/// Set pin enable state in manager
	/// \param[in]	pin		Pin to set enable state of
	void		set_pin_enabled(uint8_t pin, bool e);

private:

	/// Used to convert pin number to index in member arrays
	static uint8_t pin_nums[DIGITAL_COUNT];

	/// Convert pin number to index in manager arrays
	uint8_t		pin_to_index(uint8_t pin);

};



#endif
