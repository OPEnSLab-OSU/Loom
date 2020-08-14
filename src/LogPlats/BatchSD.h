///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_BatchSD.h
/// @brief		File for Loom_BatchSD definition.
/// @author		Adam Kerr
/// @date		2020
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "LogPlat.h"

#include <SdFat.h>



///////////////////////////////////////////////////////////////////////////////
///
/// Batch SD logging platform module.
///
/// @par Resources
/// - [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___s_d.html)
/// - [Product Page: Adafruit Adalogger Featherwing](https://www.adafruit.com/product/2922)
/// - [Product Page: Adafruit SD Breakout](https://www.adafruit.com/product/254)
/// - [Dependency: SD](https://github.com/arduino-libraries/SD)
///	- [Hardware Support](https://github.com/OPEnSLab-OSU/Loom/wiki/Hardware-Support#sd-card)
///
///////////////////////////////////////////////////////////////////////////////

class Loom_BatchSD : public LoomLogPlat
{
protected:
  SdFat sd;               ///< File System Object
  const byte chip_select; ///< Chip select pin
  int batch_counter;      ///< Current batch count value
  int packet_counter;     ///< Current packet count value in a batch
  int drop_count;         ///< Current count of packets that failed to be sent
  DynamicJsonDocument doc;
public:

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================


  /// SD Module Constructor
  ///
  /// @param[in]	enable_rate_filter			Bool | <true> | {true, false} | Whether or not to impose maximum update rate
  /// @param[in]	min_filter_delay			Int | <1000> | [100-5000] | Minimum update delay, if enable_rate_filter enabled
  /// @param[in]	chip_select					Set(Int) | <10> | {5, 6, 9, 10, 11, 12, 13, 14("A0"), 15("A1"), 16("A2"), 17("A3"), 18("A4"), 19("A5")} | Which pin to use for chip select
  Loom_BatchSD(
    LoomManager* manager,
    const bool			enable_rate_filter	= true,
    const uint16_t		min_filter_delay	= 1000,
    const byte			chip_select			= 10
  );

  /// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// @param[in]	p		The array of constuctor args to expand
  Loom_BatchSD(LoomManager* manager, JsonArrayConst p);

	/// Destructor
  ~Loom_BatchSD() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

  bool		log(JsonObject json) override { return store_batch_json(json); }

  // manually expose superclass version of log() that gets json from
	// linked LoomManager, calling this classes implementation of
	// 'log(JsonObject json)', which is pure virtual in superclass
  using LoomLogPlat::log;


  /// Clears all the files that are being stored in the batch on the SD card
  void    clear_batch_log();

  /// Accesses the internal json from Loom Manager to store to batch using store_batch_json
	/// @return True if success
  bool    store_batch();

  /// Uses JsonObject to write file as packet in the batch onto the SD Card
  /// @param[in] json   The data to be saved as .json file
  /// @return True if success
  bool    store_batch_json(JsonObject json);


  void    power_up() override;
  void    power_down() override;

  void    package(JsonObject json);

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

  void		print_config() const override;

  /// Prints the contents of a particular packet file in the batch
  /// @param[in]  index   The index of file in the batch to print
  /// @return True if success
  bool    dump_batch(int index);

//=============================================================================
///@name	GETTERS
/*@{*/ //======================================================================


  /// Returns a JsonObject of the packet file (.json) in the batch from the SD card
  /// @param[in] index    The index of the file in the batch to retrieve
  /// @return   The JsonObject stored index
  JsonObject  get_batch_json(int index);

  /// Get the current amount of packets that are being stored in the batch
  /// @return   The current number of packets
  int         get_packet_counter() { return packet_counter; }

  /// Updates the count of packets that have failed to send
  /// @param[in] dropped    The amount of packets that have been dropped during transmit
  void add_drop_count(uint8_t dropped) { drop_count += dropped; }

//=============================================================================
///@name	MISCELLANEOUS
/*@{*/ //======================================================================



private:

  /// Creates the proper file name of the file from the batch
  /// @param[in] index    The index of the file in the batch
  /// @return The file name to be used for file in batch
  void create_file_name(int index, char* name);

  /// Get the packet drop rate from the current batch
  /// Keep in mind that this drop rate will not account for retransmissions
  /// Drops are considered to be failed trainsmission or publishes used in either the Communication or Publish platforms
  /// @return The drop rate from 0 (no drops) to 100 (100% drop)
  float get_drop_rate() const;
};
