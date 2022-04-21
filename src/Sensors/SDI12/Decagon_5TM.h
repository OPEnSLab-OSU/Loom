///////////////////////////////////////////////////////////////////////////////
///
/// @file		Decagon_5TM.h
/// @brief		File for Decagon5TM definition.
/// @author		Will Richards
/// @date		2021
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_SENSORS
#pragma once

#include "SDI12_Sensor.h"
#include <SDI12.h>
#include <vector>

namespace Loom {

///////////////////////////////////////////////////////////////////////////////
///
/// Decagon GS3 soil moisture sensor module. Incomplete.
///
/// @par Resources
/// - [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___decagon_g_s3.html)
///	- [Hardware Support](https://github.com/OPEnSLab-OSU/Loom/wiki/Hardware-Support#sdi-12-sensors)
///
///////////////////////////////////////////////////////////////////////////////
class Decagon5TM : public SDI12Sensor
{

protected:

	char		sensorAddr;

	float		dielec_perm;	///< Measured dielectric permativity
	float		temp;			///< Measured temperature
	float		elec_cond;		///< Measure electrical conductivity
	
	String		sdiResponse = "";
	char		buf[20];
	char*		p;

	void parse_results();
	void clear_vectors();

public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Decagon GS3 module constructor
	///
	/// @param[in]	num_samples			Set(Int) | <8> | {1, 2, 4, 8, 16} | How many samples to take and average
	Decagon5TM(
			SDI12& sdiInterface,
			char addr,
			String moduleName
		);

	/// Destructor
	~Decagon5TM() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	void		measure() override;
	void		package(JsonObject json) override;

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void		print_config() const override;
	void		print_measurements() const override;

private:

};

///////////////////////////////////////////////////////////////////////////////
//REGISTER(Module, Decagon5TM, "Decagon5TM");
///////////////////////////////////////////////////////////////////////////////

}; // namespace Loom

#endif // ifdef LOOM_INCLUDE_SENSORS


