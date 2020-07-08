///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_IntenertLTE.h
/// @brief		File for Loom_LTE definition.
/// @author		Adam Kerr, based on work by Noah Koontz
/// @date		2020
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#pragma once

#define TINY_GSM_MODEM_SARAR4

#include <TinyGsmClient.h>
#include <ArduinoHttpClient.h>
#include "SSLClient.h"
#include "InternetPlat.h"
#include "Trust_Anchors.h"

///////////////////////////////////////////////////////////////////////////////
///
/// LTE InternetPlat
///
/// @attention	Requires 7KB of free SRAM at runtime to use.
///
/// @par Resources
/// - [Module Documentation]
/// - [Product Page: Adafruit Feather M0 Bluefruit LE](https://www.adafruit.com/product/2995)
/// - [Dependency: TinyGSM](https://github.com/vshymanskyy/TinyGSM)
/// - [Dependency: ArduinoHttpClient](//https://github.com/arduino-libraries/ArduinoHttpClient)
///	- [Hardware Support](https://github.com/OPEnSLab-OSU/Loom/wiki/Hardware-Support#cellular)
///
///////////////////////////////////////////////////////////////////////////////

class Loom_LTE: public LoomInternetPlat
{

  protected:

    const char* APN; ///< LTE Network name
    const char* gprsUser; ///< GPRS username crendtial
    const char* gprsPass; ///< GPRS password credenital
    const int powerPin;   ///< Wired analog pin to power on LTE shield

    TinyGsmClient m_base_client;  ///< SSLClient object for LTE
    SSLClient m_client;           ///< Underlying LTE SSLClient instance

    SSLClient& get_client() override {return m_client;}
    const SSLClient& get_client() const override {return m_client;}

  public:

    //==============================================================================
    ///@name	CONSTRUCTORS / DESTRUCTOR
    /*@{*/ //======================================================================

    	/// Constructor
    	/// @param[in]	APN LTE network name
      /// @param[in]	gprsUser	GPRS username. Leave as empty unless you have GPRS credentials
    	/// @param[in]	gprsPass	GPRS password. Leave as empty unless you have GPRS credentials
      /// @param[in] powerPin analog pin on the feather M0 that drives pin 5 on LTE shield to power on and off

    Loom_LTE(
        LoomManager* manager,
        const char* APN = "",
        const char* gprsUser = "",
        const char* gprsPass = "",
        const int powerPin = A5
    );

    /// Constructor that takes Json Array, extracts args
	  /// and delegates to regular constructor
	  /// @param[in]	p		The array of constuctor args to expand
    Loom_LTE(LoomManager* manager, JsonArrayConst p);

    /// Destructor
    virtual ~Loom_LTE() = default;

    //=============================================================================
    ///@name	OPERATION
    /*@{*/ //======================================================================

    /// Connect to internet
    void      connect() override;

    /// Disconnect from internet
    void      disconnect() override;

    /// Whether or not connected to internet
    /// @return True if connected, false otherwise
    bool      is_connected() const override;

    /// Fails everytime, there is no UDP socket functionality in the TinyGSM library
    /// @returns a UDP socket for transmitting and recieving
    UDPPtr open_socket(const uint port) override;

    void power_up() override;
    void power_down() override;

    //=============================================================================
    ///@name	PRINT INFORMATION
    /*@{*/ //======================================================================

    void      print_config() const override;
    void      print_state() const override;


};
