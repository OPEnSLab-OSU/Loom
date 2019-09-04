#pragma once

#include "Loom_Module.h"


///////////////////////////////////////////////////////////////////////////////


/// Abstract root of Loom sensor modules hierarchy
class LoomSensor : public LoomModule
{

protected:

	uint8_t		num_samples;	/// How many samples to take

public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	LoomSensor(	
			const char*			module_name,
			LoomModule::Type	module_type,
 			uint8_t				num_samples
		);

	/// Destructor
	virtual ~LoomSensor() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	/// Calibration routine
	virtual void	calibrate() {};

	/// Take any relevant measurements
    void Run() { measure(); }
	virtual void	measure() = 0;
	virtual void 	package(JsonObject json) override = 0;
	virtual bool	dispatch(JsonObject) {}

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	virtual void	print_config() override;
	virtual void	print_measurements() = 0; 

//=============================================================================
///@name	GETTERS
/*@{*/ //======================================================================

	/// Get the number of samples sensor is set to take
	/// \return		Number of samples
	uint8_t			get_num_samples() { return num_samples; }

//=============================================================================
///@name	SETTERS
/*@{*/ //======================================================================

	/// Set the number of samples to take
	/// \param[in]	n	Number of samples to take
	void			set_num_samples(uint8_t n) { num_samples = n; }

private:

};

