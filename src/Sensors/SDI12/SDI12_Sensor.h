///////////////////////////////////////////////////////////////////////////////
///
/// @file		SDI12_Sensor.h
/// @brief		File for SDI12Sensor definition. Incomplete.
/// @author		
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_SENSORS
#pragma once

#include "../Sensor.h"
#include <SDI12.h>
#include <Arduino.h>
#include <vector>


namespace Loom {

	///////////////////////////////////////////////////////////////////////////////
	///
	/// Abstract base class for SDI12 sensor modules.
	///
	/// @par Resources
	/// - [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom_s_d_i12_sensor.html)
	///	- [Hardware Support](https://github.com/OPEnSLab-OSU/Loom/wiki/Hardware-Support#sdi-12-sensors)
	///
	///////////////////////////////////////////////////////////////////////////////
	class SDI12Sensor : public Sensor
	{

		protected:

			// Underlying serial interface
			SDI12 SDI12Interface;

			// Register representing each sensors address connected to the sdi device
			byte addressRegister[8] = {
				0B00000000,
				0B00000000,
				0B00000000,
				0B00000000,
				0B00000000,
				0B00000000,
				0B00000000,
				0B00000000
			};

			// Comma seperated string of data readings from the SDI12 interface
			String readData = "";

			// Check if there is a activity at that set address
			bool checkActive(char i);

			// Scan the address space setting active addresses as taken
			void scanAddressSpace();

			// Checks if there is an active sensor on the address
			bool isTaken(char i);

			// Sets the specified address to taken, returns false if already taken
			bool setTaken(char i);

			// Returns a char array of all the taken addresses 
			char* getTaken();

			//Convert the ascii characters to numbers between 0 and 61 inclusive to represent the 62 possible address
			byte charToDec(char i);

			// Send a generic command over SDI 12 to a specific address
			String sendCommand(char addr, String command);

		public:
			
			//=============================================================================
			///@name	CONSTRUCTORS / DESTRUCTOR
			/*@{*/ //======================================================================

				/// Constructor
				/// @param[in]	module_name		Name of the module (provided by derived classes)
				/// @param[in]	module_type		Type of the module (provided by derived classes)
				/// @param[in]	num_samples		The number of samples to take and average
				/// @param[in]  sdiPin			Pin for the SDI12 serial to origininate from
				SDI12Sensor(int sdiPin, const char* module_name, const uint8_t num_samples = 1);

				/// Destructor
				virtual ~SDI12Sensor() = default;

			//=============================================================================
			///@name	OPERATION
			/*@{*/ //======================================================================

				void measure() override;
				void package(JsonObject json) override;
				void power_up() override;				

			//=============================================================================
			///@name	PRINT INFORMATION
			/*@{*/ //======================================================================

				void print_measurements() const override;

		private:
			void tokenize(std::string const &str, const char delim, std::vector<std::string> &out);
	///////////////////////////////////////////////////////////////////////////////

}; // namespace Loom

#endif // ifdef LOOM_INCLUDE_SENSORS

