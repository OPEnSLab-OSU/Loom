///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_Multiplexer.cpp
/// @brief		File for Loom_Multiplexer implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#include "Multiplexer.h"

// I2C Sensor files
#include "Sensors/I2C/AS7262.h"
#include "Sensors/I2C/AS7263.h"
#include "Sensors/I2C/AS7265X.h"
#include "Sensors/I2C/FXAS21002.h"
#include "Sensors/I2C/FXOS8700.h"
#include "Sensors/I2C/LIS3DH.h"
#include "Sensors/I2C/MB1232.h"
#include "Sensors/I2C/MMA8451.h"
#include "Sensors/I2C/MPU6050.h"
#include "Sensors/I2C/MS5803.h"
#include "Sensors/I2C/SHT31D.h"
#include "Sensors/I2C/TMP007.h"
#include "Sensors/I2C/TSL2561.h"
#include "Sensors/I2C/TSL2591.h"
#include "Sensors/I2C/ZXGesture.h"
#include "Sensors/I2C/STEMMA.h"

#include <Wire.h>



// Used to optimize searching for sensors:
// search addresses in array rather than 0-127
const std::array<byte, 21> Loom_Multiplexer::known_addresses =
{
	0x10, ///< ZXGESTURESENSOR
	0x11, ///< ZXGESTURESENSOR
	0x19, ///< LIS3DH
	0x1C, ///< MMA8451 / FXOS8700
	0x1D, ///< MMA8451 / FXOS8700
	0x1E, ///< FXOS8700
	0x1F, ///< FXOS8700
	0x20, ///< FXAS21002
	0x21, ///< FXAS21002
	0x29, ///< TSL2561 / TSL2591
	0x36, ///< STEMMA
	0x39, ///< TSL2561
	0x40, ///< TMP007
	0x44, ///< SHT31D
	0x45, ///< SHT31D
	0x49, ///< TSL2561 / AS726X / AS7265X
	0x68, ///< MPU6050
	0x69, ///< MPU6050
	0x70, ///< MB1232
	0x76, ///< MS5803
	0x77  ///< MS5803
};

const std::array<byte, 9>  Loom_Multiplexer::alt_addresses = {
	0x70,
	0x71,
	0x72,
	0x73,
	0x74,
	0x75,
	0x76,
	0x77,
	0x78
};

///////////////////////////////////////////////////////////////////////////////
Loom_Multiplexer::Loom_Multiplexer(
		LoomManager* 	manager,
		const byte					i2c_address,
		const uint8_t				num_ports,
		const bool					dynamic_list,
		const uint					update_period
	)
	: LoomModule(manager, "Multiplexer", Type::Multiplexer )
	, i2c_address(i2c_address)
	, num_ports(num_ports)
	, update_period(update_period)
	, sensors(new LoomI2CSensor*[num_ports])
	, control_port(num_ports)
{
  LMark;
	// Start Multiplexer
  this->power_up();
  LMark;

	// Initialize array of sensors to Null pointrs
	for (auto i = 0; i < num_ports; i++) {
    LMark;
		sensors[i] = nullptr;
    LMark;
	}

	i2c_conflicts = find_i2c_conflicts();
  LMark;

	// Update sensor list and display   -- currently removed because Mux should be linked to DeviceManager before polling sensors
	// refresh_sensors();
	// print_state();

	print_module_label();
  LMark;
	LPrintln("Setup Complete");
  LMark;
}

///////////////////////////////////////////////////////////////////////////////
Loom_Multiplexer::Loom_Multiplexer(LoomManager* manager, JsonArrayConst p)
	: Loom_Multiplexer(manager, EXPAND_ARRAY(p, 4) ) {}

///////////////////////////////////////////////////////////////////////////////
Loom_Multiplexer::~Loom_Multiplexer()
{
  LMark;
	// Free any sensors
	for (auto i = 0U; i < num_ports; i++) {
    LMark;
		if (sensors[i] != nullptr) {
      LMark;
			delete sensors[i];
      LMark;
		}
	}
	delete sensors;
}

///////////////////////////////////////////////////////////////////////////////
LoomI2CSensor* Loom_Multiplexer::generate_sensor_object(const byte i2c_address, const uint8_t port)
{
    LMark;
		LPrintln("Adding Sensor at address:", i2c_address);
    LMark;
		switch (i2c_address) {
      LMark;
			case 0x10 : return new Loom_ZXGesture(device_manager, i2c_address, port);	break;  // ZXGesture
      LMark;
			case 0x11 : return new Loom_ZXGesture(device_manager, i2c_address, port);	break;	// ZXGesture
      LMark;
			case 0x19 : return new Loom_LIS3DH(device_manager, i2c_address, port);		break;	// LIS3DH
      LMark;

			case 0x1C : // MMA8451 / FXOS8700
        LMark;
				if (i2c_0x1C == I2C_Selection::L_MMA8451)  return new Loom_MMA8451(device_manager, i2c_address, port);	// MMA8451
        LMark;
				if (i2c_0x1C == I2C_Selection::L_FXOS8700) return new Loom_FXOS8700(device_manager, i2c_address, port);	// FXOS8700
        LMark;
			break;

			case 0x1D : // MMA8451 / FXOS8700
        LMark;
				if (i2c_0x1D == I2C_Selection::L_MMA8451)  return new Loom_MMA8451(device_manager, i2c_address, port);	// MMA8451
        LMark;
				if (i2c_0x1D == I2C_Selection::L_FXOS8700) return new Loom_FXOS8700(device_manager, i2c_address, port);	// FXOS8700
        LMark;
			break;

			case 0x1E : return new Loom_FXOS8700(device_manager, i2c_address, port);	break;		// FXOS8700
      LMark;
			case 0x1F : return new Loom_FXOS8700(device_manager, i2c_address, port);	break;		// FXOS8700
      LMark;
			case 0x20 : return new Loom_FXAS21002(device_manager, i2c_address, port);	break;		// FXAS21002
      LMark;
			case 0x21 : return new Loom_FXAS21002(device_manager, i2c_address, port);	break;		// FXAS21002
      LMark;

			case 0x29 : // TSL2561 / TSL2591
        LMark;
				if (i2c_0x29 == I2C_Selection::L_TSL2561) return new Loom_TSL2561(device_manager, i2c_address, port);	// TSL2561
        LMark;
				if (i2c_0x29 == I2C_Selection::L_TSL2591) return new Loom_TSL2591(device_manager, i2c_address, port);	// TSL2591
        LMark;
			break;

			case 0x36 : return new Loom_STEMMA(device_manager, i2c_address, port);	break; // STEMMA
      LMark;
			case 0x39 : return new Loom_TSL2561(device_manager, i2c_address, port);	break;	// TSL2561
      LMark;
			case 0x40 : return new Loom_TMP007(device_manager, i2c_address, port);	break;	// TMP007
      LMark;
			case 0x44 : return new Loom_SHT31D(device_manager, i2c_address, port);	break;	// SHT31D
      LMark;
			case 0x45 : return new Loom_SHT31D(device_manager, i2c_address, port);	break;	// SHT31D
      LMark;

			case 0x49 : // TSL2561 / AS726X / AS7265X
        LMark;
				if (i2c_0x49 == I2C_Selection::L_TSL2561) return new Loom_TSL2561(device_manager, i2c_address, port);	// TSL2561
        LMark;
				if (i2c_0x49 == I2C_Selection::L_AS7262 ) return new Loom_AS7262(device_manager, i2c_address, port);		// AS7262
        LMark;
				if (i2c_0x49 == I2C_Selection::L_AS7263 ) return new Loom_AS7263(device_manager, i2c_address, port);		// AS7263
        LMark;
				if (i2c_0x49 == I2C_Selection::L_AS7265X) return new Loom_AS7265X(device_manager, i2c_address, port);	// AS7265X
        LMark;
			break;

			case 0x68 : return new Loom_MPU6050(device_manager, i2c_address, port);	break;	// MPU6050
      LMark;
			case 0x69 : return new Loom_MPU6050(device_manager, i2c_address, port);	break;	// MPU6050
      LMark;
			case 0x70 : return new Loom_MB1232(device_manager, i2c_address, port);	break;	// MB1232
      LMark;
			case 0x76 : return new Loom_MS5803(device_manager, i2c_address, port);	break;	// MS5803
      LMark;
			case 0x77 : return new Loom_MS5803(device_manager, i2c_address, port);	break;	// MS5803

			default : return nullptr;
		}
}


///////////////////////////////////////////////////////////////////////////////
void Loom_Multiplexer::print_config() const
{
  LMark;
	LoomModule::print_config();
  LMark;

	LPrint("\tI2C Address        : ");
  LMark;
	LPrintln_Hex(i2c_address);
  LMark;
	LPrintln("\tNum Ports          : ", num_ports);
  LMark;
	LPrintln("\tUpdate Period (ms) : ", update_period);
  LMark;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Multiplexer::print_state() const
{
  LMark;
	print_module_label();
  LMark;
	LPrintln("Attached Sensors:");
  LMark;

	for (auto i = 0U; i < num_ports; i++) {
    LMark;
		LPrint("\tPort ", i, ": ");
    LMark;
		if (sensors[i] != nullptr) {
      LMark;
			LPrint_Dec_Hex(sensors[i]->get_i2c_address());
      LMark;
			LPrintln(" - ", sensors[i]->get_module_name() );
      LMark;
		} else {
			LPrintln(" -");
      LMark;
		}
	}
	LPrintln();
  LMark;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Multiplexer::measure()
{
  LMark;
	refresh_sensors();
  LMark;

	for (auto i = 0U; i < num_ports; i++) {
    LMark;
		if (sensors[i] != nullptr) {
      LMark;
			tca_select(i);
      LMark;
			sensors[i]->measure();
      LMark;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Multiplexer::print_measurements() const
{
  LMark;
	for (auto i = 0U; i < num_ports; i++) {
    LMark;
		if (sensors[i] != nullptr) {
      LMark;
			tca_select(i);
      LMark;
			sensors[i]->print_measurements();
      LMark;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Multiplexer::package(JsonObject json)
{
  LMark;
	for (auto i = 0U; i < num_ports; i++) {
    LMark;
		if (sensors[i] != NULL) {
      LMark;
			tca_select(i);
      LMark;
			sensors[i]->package(json);
      LMark;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Multiplexer::diagnose(bool& result){
  LMark;
	// implement here
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Multiplexer::get_sensor_list(JsonObject json)
{
  LMark;
	refresh_sensors();
  LMark;

	json["type"] = "state";
  LMark;
	JsonObject list = json.createNestedObject("MuxSensors");
  LMark;

	char tmp[3];
  LMark;
	for (auto i = 0U; i < num_ports; i++) {
    LMark;
		if (sensors[i] != NULL) {
      LMark;
			itoa(i, tmp, 10);
      LMark;
			list[tmp] = sensors[i]->get_module_name();
      LMark;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Multiplexer::refresh_sensors()
{
  LMark;
	byte previous, current;
  LMark;

    // update conflicts
    i2c_conflicts = find_i2c_conflicts();
  LMark;

	for (auto i = 0; i < num_ports; i++) {
    LMark;

		// LPrintln("TCA Port: ", i);
		// tca_select(i);

		previous = (sensors[i] != nullptr) ? sensors[i]->get_i2c_address() : 0x00;
    LMark;
		// LPrint("\tPrevious: 0x");
		// LPrintln_Hex(previous);

		current = get_i2c_on_port(i);
    LMark;

		// LPrint("Current I2C on port ", i, " : ");
		// LPrintln_Dec_Hex(current);

		// Cases:
		// No change (prev = current)
			// Do nothing
		// Sensor removed (prev ≠ 0, current = 0)
			// Free removed sensor object memory
		// Sensor added (prev = 0, currect ≠ 0)
			// Create object
		// Sensor switched (prev ≠ current)
			// Delete old object
			// Create new object

		// Sensor removed, added, or swapped
		if (previous != current) {
      LMark;

			if ( (previous != 0) || (current == 0) ) {
        LMark;
				// Free object
				print_module_label();
        LMark;
				LPrintln("Free Memory of ", sensors[i]->get_module_name() );
        LMark;
				delete sensors[i];
        LMark;
			}

			// Create new sensor object and setup (in constructor)
			sensors[i] = generate_sensor_object(current, i);
      LMark;

			if (sensors[i] != nullptr) {
        LMark;

				if (sensors[i]->get_active()) {
          LMark;

					// Make sure sensor is also linked to DeviceManager
					sensors[i]->link_device_manager(device_manager);
          LMark;
					// device_manager->add_module(sensors[i]);


					print_module_label();
          LMark;
					LPrintln("Added ", sensors[i]->get_module_name() );
          LMark;
				} else {
					// Sensors will switch themselves to inactive if they dont
					// properly initialize.
					// If so, don't add sensor
					print_module_label();
          LMark;
					LPrintln(sensors[i]->get_module_name(), " failed to initialize");
          LMark;

					delete sensors[i];
          LMark;
					sensors[i] = nullptr;
          LMark;
				}

			}
		}

		// Otherwise still no sensor or sensor did not
		// change. In which case, do nothing.
	}
}

///////////////////////////////////////////////////////////////////////////////
byte Loom_Multiplexer::get_i2c_on_port(const uint8_t port) const
{
  LMark;
	tca_select(port);
  LMark;
	byte addr;
  LMark;

	// Iterate through known addresses try to get confirmation from sensor
	// for (auto addr = 1; addr <= 127; addr++) {
	for (const auto addr : known_addresses) {
         LMark;
        // if this address is on the conflict list, skip it
        if (i2c_conflict(addr) || addr == this->i2c_address) { continue; }

		Wire.beginTransmission(addr);
    LMark;
		byte error = Wire.endTransmission();
    LMark;

		if (error == 0) return addr;
    LMark;
	}

	return 0x00; // No sensor found
}

bool Loom_Multiplexer::i2c_conflict(byte addr) const {
    LMark;
    for (byte conflict : i2c_conflicts)
        if (conflict == addr)
            return true;
    LMark;
    return false;
}

std::vector<byte> Loom_Multiplexer::find_i2c_conflicts() {
    LMark;

    tca_deselect();
    LMark;
    std::vector<byte> i2c_conflicts_local;
    LMark;
    byte addr;
    LMark;
    // go through all the potentially conflicting sensors and find the ones that respond to blacklist them.
    for (auto j = 0; j < known_addresses.size(); j++) {
        LMark;

        addr = known_addresses[j];
        LMark;

        Wire.beginTransmission(addr);
        LMark;
        byte error = Wire.endTransmission();
        LMark;

        if (error == 0) {
            LMark;
            i2c_conflicts_local.push_back(addr);
            LMark;
        }
    }

    return i2c_conflicts_local;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Multiplexer::tca_select(const uint8_t port) const
{
  LMark;
	if (port < num_ports) {
    LMark;
		Wire.beginTransmission(i2c_address);
    LMark;
		Wire.write(1 << port);
    LMark;
		Wire.endTransmission();
    LMark;
	}
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Multiplexer::tca_deselect() const
{
  LMark;
	Wire.beginTransmission(i2c_address);
  LMark;
	Wire.write(0);
  LMark;
	Wire.endTransmission();
  LMark;
}

///////////////////////////////////////////////////////////////////////////////

void Loom_Multiplexer::power_up() {
  LMark;
// Begin I2C
	Wire.begin();
  LMark;

	Wire.beginTransmission(this->i2c_address);
  LMark;
	if(Wire.endTransmission() ) { //< Test on this address
    LMark;
		//< Test Failed
		LPrintln("Multiplexer not found on specified port. Checking alternate addresses.");
    LMark;
		this->active = false;
    LMark;

		//< Check all alternate addresses
		for(auto address : alt_addresses) {
      LMark;
			Wire.beginTransmission(address);
      LMark;
			if(Wire.endTransmission()) {
        LMark;
				continue;
			} else {
				active = true;
        LMark;
				this->i2c_address = address;
        LMark;
				LPrintln("*** Multiplexer found at: ", address, ", update your config. ***");
        LMark;
			}
		}
	}
	//< Test succeded
	else
		this->active = true;
}

///////////////////////////////////////////////////////////////////////////////

void Loom_Multiplexer::power_down() {

}
