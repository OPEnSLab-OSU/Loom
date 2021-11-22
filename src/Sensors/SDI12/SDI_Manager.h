///////////////////////////////////////////////////////////////////////////////
///
/// @file		SDI_Manager.h
/// @brief		File for SDI_Manager definition.
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
#include <map>

namespace Loom {

	///////////////////////////////////////////////////////////////////////////////
	///
	/// Manager for all connected SDI12 Devices
	///
	/// @par Resources
	/// - [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___decagon_g_s3.html)
	///	- [Hardware Support](https://github.com/OPEnSLab-OSU/Loom/wiki/Hardware-Support#sdi-12-sensors)
	///
	///////////////////////////////////////////////////////////////////////////////
	class SDI_Manager : public SDI12Sensor
	{

		protected:
			const uint8_t				pinAddr; // Pin on the feather that SDI12 is running off

			std::map<char, String> 			sensorsInfo; // Mapping sensor address to sensor version
			std::vector<SDI12Sensor*> 		sensors; // Vector of actual sensor classes

			// Construct the used sensors and add them to a vector
			void construct_sensors();

		public:
			
		//=============================================================================
		///@name	CONSTRUCTORS / DESTRUCTOR
		/*@{*/ //======================================================================

			/// Decagon GS3 module constructor
			///
			/// @param[in]	sdiPin			Pin on the board that runs the SDI12 Bus
			SDI_Manager(
					const uint8_t sdiPin = 11
				);

			/// Constructor that takes Json Array, extracts args
			/// and delegates to regular constructor
			/// @param[in]	p		The array of constuctor args to expand
			SDI_Manager(JsonArrayConst p);

			/// Destructor
			~SDI_Manager() = default;

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
	REGISTER(Module, SDI_Manager, "SDIManager");
	///////////////////////////////////////////////////////////////////////////////

}; // namespace Loom

#endif // ifdef LOOM_INCLUDE_SENSORS


