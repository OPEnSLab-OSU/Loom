///////////////////////////////////////////////////////////////////////////////
///
/// @file		RFID.h
/// @brief		File for RFID Reader definition.
/// @author		Adam Kerr
/// @date		2020
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Serial_Sensor.h"
#include "SparkFun_UHF_RFID_Reader.h"

///////////////////////////////////////////////////////////////////////////////
///
/// RFID Reader module
///
/// @par Resources
/// - [Module Documentation]
/// - [Product Page: SparkFun Simultaneous RFID Reader - M6E Nano](https://www.sparkfun.com/products/14066)
/// - [Datasheet: RFID Reader](https://cdn.sparkfun.com/assets/4/e/5/5/0/SEN-14066_datasheet.pdf)
/// - [Hookup Guide](https://learn.sparkfun.com/tutorials/simultaneous-rfid-tag-reader-hookup-guide/hardware-hookup)
///	- [Hardware Support](https://github.com/OPEnSLab-OSU/Loom/wiki/Hardware-Support)
///
///////////////////////////////////////////////////////////////////////////////

class Loom_RFID : public LoomSerialSensor
{

public:

  enum class MeasureType {
    Unknown,
    Moisture
  };

  enum class State {
    COMPARE_THRESHOLD,
    WET_CYCLE,
    DRY_CYCLE,
 };

  typedef struct{
     MeasureType type;
     byte EPCHeaderName;
     byte fertigate;
     uint8_t threshold;
     State state = State::COMPARE_THRESHOLD;
     int rssi;
     long freq;
     long moisture;
     byte tagEPCBytes;

 } tag;

protected:
  boolean     setupNano();
  void        moistureTagMeasurement();
  void        addNewTag(int i, byte EPCHeader, byte EPCFertigate);
  void        updateData(int i, int rssi, long freq, long moisture, byte tagEPCBytes, byte EPCFertigate);

  Uart                rfid_serial;  ///< Uart connect used by the RFID reader
  RFID                nano;         ///< Underlying RFID nano tag reader instance
  const MeasureType   type;         ///< Defines the type of RFID tag the reader should measure and package data for
  const int           num_tags;     ///< Defines the number of RFID tags the reader should expect to measure
  const int           MAX_VAL;      ///< Defines the maximum value for the tag measurements to be considered in the high state
  const int           MIN_VAL;      ///< Defines the minumum value for the tag measurements to be considered in the low state
  tag *               rfid_tags;    ///< The array of tags that have been found by the RFID Reader
  int                 tag_counter;  ///< The counter for the number of tags that have been found by the RFID Reader
  int                 index;        ///< The index of the last RFID tag that has been read
  boolean             updated;      ///< Defnies whether or not the last RFID tag was previously found and now updated

public:
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

    /// RFID Reader constructor
    ///
    /// @param[in] t    Defines which type of tags are being read, only Moisture is supported at this time
    /// @param[in] tags Defines the number of tags that are expected to be read.
    /// @param[in] max  Defines the maximum value for the tag measurements to be considered in the high state
    /// @param[in] min  Defines the minumum value for the tag measurements to be considered in the low state
    /// @param[in] RX   The RX pin that is being used on the Feather M0 to connect with the RFID Tag Reader
    /// @param[in] TX   The RX pin that is being used on the Feather M0 to connect with the RFID Tag Reader
    /// @param[in] num_samples    Set(Int) | <8> | {1, 2, 4, 8, 16} | How many samples to take and average
    Loom_RFID(LoomManager* manager,
        MeasureType t = MeasureType::Moisture,
        int tags=10,
        int max=26,
        int min=17,
        int RX=12,
        int TX=11,
        int num_samples = 1
        );

    /// Constructor that takes Json Array, extracts args
    /// and delegates to regular constructor
    /// @param[in]	p		The array of constuctor args to expand
    Loom_RFID(LoomManager* manager, JsonArrayConst p);

    /// Destructor
    ~Loom_RFID();

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

    void        measure() override;
    void        package(JsonObject json) override;
    boolean     isUpdatedIndex();
    int         getTagIndex();
    void        setTag(int i, tag t);
    tag         getTag(int i);


//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

    void    print_measurements()   const override;


};
