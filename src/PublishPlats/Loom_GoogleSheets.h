#pragma once

#include "Loom_PublishPlat.h"
#include "ArduinoJson.h"


///////////////////////////////////////////////////////////////////////////////


class Loom_GoogleSheets : public LoomPublishPlat {

protected:
	
	
public:

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Loom Google Sheets Platform module constructor.
	/// Check out http://www.open-sensing.org/evaporometerblog/datalog for more information on these parameters.
	/// \param[in]	module_name		String | <"Internet-Plat"> | null | Publish platform module name.
	/// \param[in]  internet_index	Int | <0> | [0-5] | Index from zero of of the desired internet platform based on the JSON configuration. 
	/// \param[in]  script_url		String | <""> | null | URL where the google sheets script is hosted, without domain (ex. /macros/s/AKfycby<more letters here>_k0n/exec).
	/// \param[in]  sheet_id		String | <""> | null |ID Of the spreadsheet to put data in (script must have edit access).
	/// \param[in]  tab_id			String | <"Data"> | null | Name of the tab that the device will put data into. 
	/// \param[in]  device_id		String | <"Mine"> | null | Identifier to use for the device in the spreadsheet.
	Loom_GoogleSheets(	
						const char* module_name,
						const uint	internet_index,
						const char* script_url,
						const char* sheet_id,
						const char* tab_id = "Data",
						const char* device_id = "Mine"
					);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// \param[in]  p     The array of constuctor args to expand
	Loom_GoogleSheets( JsonArrayConst p );

	/// Destructor
	~Loom_GoogleSheets() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	// remember to close the socket!
	bool send_to_internet(const JsonObject json, LoomInternetPlat* plat) override;

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void print_config() override;

private:

	/// Get the number of bytes left in buffer from a given pointer in buffer
	size_t m_buffer_left(const char* buffer_index) const {
		const char* end_buffer = &(m_buffer[sizeof(m_buffer) - 1]);
		if (buffer_index >= end_buffer) return 0;
		return end_buffer - buffer_index;
	}

	/// Private utility send function
	bool m_serialize_internet_impl(const JsonObject json);

	/// Storage for all those strings we're going to need above
	/// main string buffer, the front of which stores our URL
	/// do not edit this directly, use m_url_end instead.
	char m_buffer[768];
	
	/// Store a pointer to where we can actually add data
	char* m_data_start;
};

