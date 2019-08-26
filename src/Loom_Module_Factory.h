#pragma once

#include "Loom_Module.h"

#include <ArduinoJson.h>


// Actuators
#include "Actuators/Loom_Neopixel.h"
#include "Actuators/Loom_Relay.h"
#include "Actuators/Loom_Servo.h"
#include "Actuators/Loom_Stepper.h"

// CommPlats
#include "CommPlats/Loom_LoRa.h"
#include "CommPlats/Loom_nRF.h"
#include "CommPlats/Loom_Bluetooth.h"

// InternetPlats
#include "InternetPlats/Loom_InternetEthernet.h"
#include "InternetPlats/Loom_InternetWiFi.h"

// PublishPlats
#include "PublishPlats/Loom_GoogleSheets.h"
#include "PublishPlats/Loom_Max_Pub.h"

// SubscribePlats
#include "SubscribePlats/Loom_Max_Sub.h"

// LogPlats
#include "LogPlats/Loom_OLED.h"
#include "LogPlats/Loom_SD.h"

// // RTC
#include "RTC/Loom_DS3231.h"
#include "RTC/Loom_PCF8523.h"

// Sensors
#include "Sensors/Loom_Analog.h"
#include "Sensors/Loom_Digital.h"

#include "Sensors/I2C/Loom_AS7262.h"
#include "Sensors/I2C/Loom_AS7263.h"
#include "Sensors/I2C/Loom_AS7265X.h"
#include "Sensors/I2C/Loom_FXAS21002.h"
#include "Sensors/I2C/Loom_FXOS8700.h"
#include "Sensors/I2C/Loom_LIS3DH.h"
#include "Sensors/I2C/Loom_MB1232.h"
#include "Sensors/I2C/Loom_MMA8451.h"
#include "Sensors/I2C/Loom_MPU6050.h"
#include "Sensors/I2C/Loom_MS5803.h"
#include "Sensors/I2C/Loom_SHT31D.h"
#include "Sensors/I2C/Loom_TMP007.h"
#include "Sensors/I2C/Loom_TSL2561.h"
#include "Sensors/I2C/Loom_TSL2591.h"
#include "Sensors/I2C/Loom_ZXGesture.h"

#include "Sensors/SDI12/Loom_Decagon_5TM.h"
#include "Sensors/SDI12/Loom_Decagon_GS3.h"

#include "Sensors/SPI/Loom_MAX31855.h"
#include "Sensors/SPI/Loom_MAX31856.h"

// Other
#include "Loom_NTP_Sync.h"

// General
#include "Loom_Interrupt_Manager.h"
#include "Loom_Sleep_Manager.h"
#include "Loom_Multiplexer.h" // this needs to be include after I2C sensors (due to conflict with enableInterrupt macro/function defined by EnableInterrupt library and AS726X sensors)



#undef min
#undef max
#include <array>
#include <tuple>



/// Creates a LoomModule with default parameters
/// \return The created LoomModule
template<class T> LoomModule* ConstructDefault() { return new T(); }

/// Creates a LoomModule with Json array of parameters
/// \return The created LoomModule
template<class T> LoomModule* Construct(JsonArrayConst p) { return new T(p); }



///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////





// Fill in missing stuff
namespace ersatz {

template<std::size_t I, typename T> using tuple_element_t = typename std::tuple_element<I, T>::type;
template<typename T> using remove_reference_t = typename std::remove_reference<T>::type;
template<typename... T> using common_type_t = typename std::common_type<T...>::type;

template<typename Int, Int... Seq> struct integer_sequence {
	using value_type = Int;
	static constexpr std::size_t size() noexcept { return sizeof...(Seq); }
};

template<std::size_t... Seq> using index_sequence = integer_sequence<std::size_t, Seq...>;

template<typename Int, std::size_t Acc, Int... Seq> struct build_sequence { using type = typename build_sequence<Int, Acc - 1, Acc - 1, Seq...>::type; };
template<typename Int, Int... Seq> struct build_sequence<Int, 0, Seq...> { using type = integer_sequence<Int, Seq...>; };

template<typename Int, Int N> using make_integer_sequence = typename build_sequence<Int, N>::type;
template<std::size_t N> using make_index_sequence = make_integer_sequence<std::size_t, N>;
	
} // ersatz

template<
	typename Tuple,
	typename VTuple = ersatz::remove_reference_t<Tuple>,
	std::size_t... Indices
>
constexpr std::array<
	ersatz::common_type_t<ersatz::tuple_element_t<Indices, VTuple>...>,
	sizeof...(Indices)
>
to_array(Tuple&& tuple, ersatz::index_sequence<Indices...>) 
{
	return { std::get<Indices>(std::forward<Tuple>(tuple))... };
}

template<typename Tuple, typename VTuple = ersatz::remove_reference_t<Tuple>>
constexpr auto to_array(Tuple&& tuple) 
{
	return to_array(
		std::forward<Tuple>(tuple),
		ersatz::make_index_sequence<std::tuple_size<VTuple>::value> {} );
}



///////////////////////////////////////////////////////////////////////////////


// probably rename to avoid confusion with normal get

using namespace std;

template<size_t index, typename T, typename... Ts>
inline constexpr typename std::enable_if<index==0, T>::type
get(T&& t, Ts&&... ts) {
	return t;
}

template<size_t index, typename T, typename... Ts>
inline constexpr typename std::enable_if<(index > 0) && index <= sizeof...(Ts),
		  typename tuple_element<index, tuple<T, Ts...>>::type>::type
get(T&& t, Ts&&... ts) {
	return get<index-1>(std::forward<Ts>(ts)...);
}

// below is optional - just for getting a more readable compilation error
// in case calling get with a bad index

// inline template<long long index, typename... Ts>
template<long long index, typename... Ts>
constexpr bool index_ok() {
	return index >= 0 && index < sizeof...(Ts);
}

template<long long index, typename T, typename... Ts>
inline constexpr
typename std::enable_if<!index_ok<index, T, Ts...>(), T>::type
get(T&& t, Ts&&... ts) {
	static_assert(index_ok<index, T, Ts...>(),
		"bad index in call to get, smaller than zero or above pack size");
	return t;
}




///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////





/// Function pointer to 'template<class T> LoomModule* Construct(JsonArrayConst p)'
using FactoryPtr = LoomModule* (*)(JsonArrayConst p);
using FactoryPtrDefault = LoomModule* (*)();

/// Struct to contain the elements of factory lookup table
typedef struct {
	const char*			name;				// Module type to compare against
	FactoryPtr			Construct;			// Pointer to 'template<class T> LoomModule* Create(JsonArrayConst p)' with the type T set
	FactoryPtrDefault	ConstructDefault;	// Pointer to 'template<class T> LoomModule* CreateDefault()' with the type T set
} NameModulePair;
















///////////////////////////////////////////////////////////////////////////////


/// LoomModule Factory
class Factory 
{

public:

	/// Constructor
	Factory() = default;
	
	/// Destructor
	~Factory() = default;

	/// Create a LoomModule accoding to Json parameters
	/// \param[in]	module			Json of the module name and settings
	/// \return The created LoomModule
	LoomModule* Create(JsonVariant module);

	/// Creates a LoomModule with its default parameters.
	/// Usage example:
	///		Loom_Relay r = FactoryInst.CreateDefault<Loom_Relay>();
	/// \return The created LoomModule
	template<class T> 
	T* CreateDefault() {
		return ConstructDefault<T>();
	}



public: //make private again later

	// /// Function pointer to 'template<class T> LoomModule* Construct(JsonArrayConst p)'
	// using FactoryPtr = LoomModule* (*)(JsonArrayConst p);
	// using FactoryPtrDefault = LoomModule* (*)();

	// /// Struct to contain the elements of factory lookup table
	// typedef struct {
	// 	const char*			name;				// Module type to compare against
	// 	FactoryPtr			Construct;			// Pointer to 'template<class T> LoomModule* Create(JsonArrayConst p)' with the type T set
	// 	FactoryPtrDefault	ConstructDefault;	// Pointer to 'template<class T> LoomModule* CreateDefault()' with the type T set
	// } NameModulePair;

	/// Factory lookup table
	const static NameModulePair LookupTable[];




};




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



namespace Enable 
{
	enum class Internet {All, Ethernet, WiFi, None };
	enum class Sensors { Enabled, Disabled };
	enum class Actuators { Enabled, Disabled };
	enum class Radios { Enabled, Disabled };
	enum class Max { Enabled, Disabled };
}


namespace Include
{
	// Empty block
	constexpr auto None = std::make_tuple();

	// Common
	constexpr auto Common = std::make_tuple( 
				NameModulePair{"Interrupt_Manager",	Construct<Loom_Interrupt_Manager>,		ConstructDefault<Loom_Interrupt_Manager> },		// < 1%
				NameModulePair{"Sleep_Manager",		Construct<Loom_Sleep_Manager>,			ConstructDefault<Loom_Sleep_Manager> },			// < 1%
				NameModulePair{"Analog",			Construct<Loom_Analog>,					ConstructDefault<Loom_Analog> },				// 4%
				NameModulePair{"Digital",			Construct<Loom_Digital>,				ConstructDefault<Loom_Digital> },				// < 1%	
				// LogPlat
				NameModulePair{"OLED",				Construct<Loom_OLED>,					ConstructDefault<Loom_OLED> },					// 4%
				NameModulePair{"SD",				Construct<Loom_SD>,						ConstructDefault<Loom_SD> },					// 3%
				// // RTC
				NameModulePair{"DS3231",			Construct<Loom_DS3231>,					ConstructDefault<Loom_DS3231> },					// < 1%
				NameModulePair{"PCF8523",			Construct<Loom_PCF8523>,				ConstructDefault<Loom_PCF8523> }				// < 1%
			);

	// Radios
	constexpr auto Radios = std::make_tuple( 
				NameModulePair{"LoRa",			Construct<Loom_LoRa>,			ConstructDefault<Loom_LoRa> },				// 4%
				NameModulePair{"nRF",			Construct<Loom_nRF>,			ConstructDefault<Loom_nRF> },				// 4%
				NameModulePair{"Bluetooth",		Construct<Loom_Bluetooth>,		ConstructDefault<Loom_Bluetooth> }			// 1%
			);

	// Max
	constexpr auto Max = std::make_tuple( 
				NameModulePair{"MaxPub",		Construct<Loom_MaxPub>,			nullptr },									// 1%
				NameModulePair{"MaxSub",		Construct<Loom_MaxSub>,			nullptr }									// 16%   10% if all internet being used
			);


	// Ethernet and WiFi
	constexpr auto EthernetAndWiFi = std::make_tuple( 
				NameModulePair{"Ethernet",		Construct<Loom_Ethernet>,		ConstructDefault<Loom_Ethernet> },
				NameModulePair{"WiFi",			Construct<Loom_WiFi>,			ConstructDefault<Loom_WiFi> },				// 6%  (none if Max being used)
				NameModulePair{"GoogleSheets",	Construct<Loom_GoogleSheets>,	nullptr },									// 1%
				NameModulePair{"NTP_Sync", 		Construct<LoomNTPSync>,			ConstructDefault<LoomNTPSync> }
			);

	// Ethernet
	constexpr auto Ethernet = std::make_tuple( 
				NameModulePair{"Ethernet",		Construct<Loom_Ethernet>,		ConstructDefault<Loom_Ethernet> },
				NameModulePair{"GoogleSheets",	Construct<Loom_GoogleSheets>,	nullptr },									// 1%
				NameModulePair{"NTP_Sync", 		Construct<LoomNTPSync>,			ConstructDefault<LoomNTPSync> }
			);

	// WiFi
	constexpr auto WiFi = std::make_tuple( 
				NameModulePair{"WiFi",			Construct<Loom_WiFi>,			ConstructDefault<Loom_WiFi> },
				NameModulePair{"GoogleSheets",	Construct<Loom_GoogleSheets>,	nullptr },									// 1%
				NameModulePair{"NTP_Sync", 		Construct<LoomNTPSync>,			ConstructDefault<LoomNTPSync> }
			);


	// Sensors
	constexpr auto Sensors = std::make_tuple(
				NameModulePair{"Multiplexer",	Construct<Loom_Multiplexer>,	ConstructDefault<Loom_Multiplexer> },
				// I2C
				NameModulePair{"AS7262",		Construct<Loom_AS7262>,			ConstructDefault<Loom_AS7262> },
				NameModulePair{"AS7263",		Construct<Loom_AS7263>,			ConstructDefault<Loom_AS7263> },
				NameModulePair{"AS7265X",		Construct<Loom_AS7265X>,		ConstructDefault<Loom_AS7265X> },
				NameModulePair{"FXAS21002",		Construct<Loom_FXAS21002>,		ConstructDefault<Loom_FXAS21002> },
				NameModulePair{"FXOS8700",		Construct<Loom_FXOS8700>,		ConstructDefault<Loom_FXOS8700> },
				NameModulePair{"LIS3DH",		Construct<Loom_LIS3DH>,			ConstructDefault<Loom_LIS3DH> },
				NameModulePair{"MB1232",		Construct<Loom_MB1232>,			ConstructDefault<Loom_MB1232> },
				NameModulePair{"MMA8451",		Construct<Loom_MMA8451>,		ConstructDefault<Loom_MMA8451> },
				NameModulePair{"MPU6050",		Construct<Loom_MPU6050>,		ConstructDefault<Loom_MPU6050> },
				NameModulePair{"MS5803",		Construct<Loom_MS5803>,			ConstructDefault<Loom_MS5803> },
				NameModulePair{"SHT31D",		Construct<Loom_SHT31D>,			ConstructDefault<Loom_SHT31D> },
				NameModulePair{"TMP007",		Construct<Loom_TMP007>,			ConstructDefault<Loom_TMP007> },
				NameModulePair{"TSL2561",		Construct<Loom_TSL2561>,		ConstructDefault<Loom_TSL2561> },
				NameModulePair{"TSL2591",		Construct<Loom_TSL2591>,		ConstructDefault<Loom_TSL2591> },
				NameModulePair{"ZXGesture",		Construct<Loom_ZXGesture>,		ConstructDefault<Loom_ZXGesture> },
				// SDI12
				NameModulePair{"Decagon5TM",	Construct<Loom_Decagon5TM>,		ConstructDefault<Loom_Decagon5TM> },
				NameModulePair{"DecagonGS3",	Construct<Loom_DecagonGS3>,		ConstructDefault<Loom_DecagonGS3> },
				// SPI
				NameModulePair{"MAX31855",		Construct<Loom_MAX31855>,		ConstructDefault<Loom_MAX31855> },
				NameModulePair{"MAX31856",		Construct<Loom_MAX31856>,		ConstructDefault<Loom_MAX31856> }
			);


	// Actuators
	constexpr auto Actuators = std::make_tuple( 
				NameModulePair{"Neopixel",		Construct<Loom_Neopixel>,		ConstructDefault<Loom_Neopixel> },
				NameModulePair{"Relay",			Construct<Loom_Relay>,			ConstructDefault<Loom_Relay> },
				NameModulePair{"Servo",			Construct<Loom_Servo>,			ConstructDefault<Loom_Servo> },
				NameModulePair{"Stepper",		Construct<Loom_Stepper>,		ConstructDefault<Loom_Stepper> }
			);

} // end Include namespace






// make use of default parameters
template<
	Enable::Internet INTERNET	= Enable::Internet::All,
	Enable::Sensors SENSORS		= Enable::Sensors::Enabled,
	Enable::Radios RADIOS		= Enable::Radios::Enabled,
	Enable::Actuators ACTUATORS	= Enable::Actuators::Enabled,
	Enable::Max MAX				= Enable::Max::Disabled
>
class NewFactory 
{

private:

	static constexpr auto LookupTable = tuple_cat(
			Include::Common,
			get<(int)INTERNET>(			
				Include::EthernetAndWiFi,
				Include::Ethernet,
				Include::WiFi,
				Include::None
			), 
			get<(int)SENSORS>(		
				Include::Sensors,
				Include::None
			),
			get<(int)RADIOS>(		
				Include::Radios,
				Include::None
			),
			get<(int)ACTUATORS>(		
				Include::Actuators,
				Include::None
			),
			get<(int)MAX>(		
				Include::Max,
				Include::None
			)
		);

public:

	static void print_table()
	{
		LPrintln("Module Lookup Table:\n");

		for (auto item : to_array(LookupTable) ) {
			LPrintln(item.name);
		}
	}


};







