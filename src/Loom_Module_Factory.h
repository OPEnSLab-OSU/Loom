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







// move these to their own file


///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////

#undef min
#undef max
#include <array>
#include <tuple>



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


























/// Creates a LoomModule with default parameters
/// \return The created LoomModule
template<class T> LoomModule* ConstructDefault() { return new T(); }

/// Creates a LoomModule with Json array of parameters
/// \return The created LoomModule
template<class T> LoomModule* Construct(JsonArrayConst p) { return new T(p); }

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

	/// Function pointer to 'template<class T> LoomModule* Construct(JsonArrayConst p)'
	using FactoryPtr = LoomModule* (*)(JsonArrayConst p);
	using FactoryPtrDefault = LoomModule* (*)();

	/// Struct to contain the elements of factory lookup table
	typedef struct {
		const char*			name;				// Module type to compare against
		FactoryPtr			Construct;			// Pointer to 'template<class T> LoomModule* Create(JsonArrayConst p)' with the type T set
		FactoryPtrDefault	ConstructDefault;	// Pointer to 'template<class T> LoomModule* CreateDefault()' with the type T set
	} NameModulePair;

	/// Factory lookup table
	const static NameModulePair LookupTable[];





	// constexpr auto LookupTable2 = to_array(std::tuple_cat(
			
	// 		// Common
	// 		get<0>(
	// 			std::make_tuple( 
	// 				Factory::NameModulePair{"Interrupt_Manager",	Construct<Loom_Interrupt_Manager>,		ConstructDefault<Loom_Interrupt_Manager> },
	// 				Factory::NameModulePair{"Sleep_Manager",		Construct<Loom_Sleep_Manager>,			ConstructDefault<Loom_Sleep_Manager> },
	// 				Factory::NameModulePair{"Analog",				Construct<Loom_Analog>,					ConstructDefault<Loom_Analog> },
	// 				Factory::NameModulePair{"Digital",				Construct<Loom_Digital>,				ConstructDefault<Loom_Digital> },
	// 				// CommPlat
	// 				Factory::NameModulePair{"LoRa",					Construct<Loom_LoRa>,					ConstructDefault<Loom_LoRa> },
	// 				Factory::NameModulePair{"nRF",					Construct<Loom_nRF>,					ConstructDefault<Loom_nRF> },
	// 				Factory::NameModulePair{"Bluetooth",			Construct<Loom_Bluetooth>,				ConstructDefault<Loom_Bluetooth> },
	// 				// PublishPlat
	// 				Factory::NameModulePair{"GoogleSheets",			Construct<Loom_GoogleSheets>,			nullptr },
	// 				Factory::NameModulePair{"MaxPub",				Construct<Loom_MaxPub>,					nullptr },
	// 				// SubscribePlat
	// 				Factory::NameModulePair{"MaxSub",				Construct<Loom_MaxSub>,					nullptr },
	// 				// LogPlat
	// 				Factory::NameModulePair{"OLED",					Construct<Loom_OLED>,					ConstructDefault<Loom_OLED> },
	// 				Factory::NameModulePair{"SD",					Construct<Loom_SD>,						ConstructDefault<Loom_SD> },
	// 				// RTC
	// 				Factory::NameModulePair{"DS3231",				Construct<Loom_DS3231>,					ConstructDefault<Loom_DS3231> },
	// 				Factory::NameModulePair{"PCF8523",				Construct<Loom_PCF8523>,				ConstructDefault<Loom_PCF8523> }
			
	// 			),
	// 			std::make_tuple()
	// 		),


	// 		// Internet
	// 		get<0>(
	// 			std::make_tuple( // Both Ethernet and WiFi
	// 				Factory::NameModulePair{"Ethernet",		Construct<Loom_Ethernet_I>,		ConstructDefault<Loom_Ethernet_I> },
	// 				Factory::NameModulePair{"WiFi",			Construct<Loom_WiFi_I>,			ConstructDefault<Loom_WiFi_I> },
	// 				Factory::NameModulePair{"NTP_Sync", 	Construct<LoomNTPSync>,			ConstructDefault<LoomNTPSync> }
	// 			),
	// 			std::make_tuple( // Only WiFi
	// 				Factory::NameModulePair{"WiFi",			Construct<Loom_WiFi_I>,			ConstructDefault<Loom_WiFi_I> },
	// 				Factory::NameModulePair{"NTP_Sync", 	Construct<LoomNTPSync>,			ConstructDefault<LoomNTPSync> }
	// 			),
	// 			std::make_tuple( // Only Ethernet
	// 				Factory::NameModulePair{"Ethernet",		Construct<Loom_Ethernet_I>,		ConstructDefault<Loom_Ethernet_I> },
	// 				Factory::NameModulePair{"NTP_Sync", 	Construct<LoomNTPSync>,			ConstructDefault<LoomNTPSync> }
	// 			),
	// 			std::make_tuple() // No internet
	// 		),


	// 		// Sensors
	// 		get<0>(
	// 			std::make_tuple(
	// 				Factory::NameModulePair{"Multiplexer",	Construct<Loom_Multiplexer>,	ConstructDefault<Loom_Multiplexer> },

	// 				Factory::NameModulePair{"AS7262",		Construct<Loom_AS7262>,			ConstructDefault<Loom_AS7262> },
	// 				Factory::NameModulePair{"AS7263",		Construct<Loom_AS7263>,			ConstructDefault<Loom_AS7263> },
	// 				Factory::NameModulePair{"AS7265X",		Construct<Loom_AS7265X>,		ConstructDefault<Loom_AS7265X> },
	// 				Factory::NameModulePair{"FXAS21002",	Construct<Loom_FXAS21002>,		ConstructDefault<Loom_FXAS21002> },
	// 				Factory::NameModulePair{"FXOS8700",		Construct<Loom_FXOS8700>,		ConstructDefault<Loom_FXOS8700> },
	// 				Factory::NameModulePair{"LIS3DH",		Construct<Loom_LIS3DH>,			ConstructDefault<Loom_LIS3DH> },
	// 				Factory::NameModulePair{"MB1232",		Construct<Loom_MB1232>,			ConstructDefault<Loom_MB1232> },
	// 				Factory::NameModulePair{"MMA8451",		Construct<Loom_MMA8451>,		ConstructDefault<Loom_MMA8451> },
	// 				Factory::NameModulePair{"MPU6050",		Construct<Loom_MPU6050>,		ConstructDefault<Loom_MPU6050> },
	// 				Factory::NameModulePair{"MS5803",		Construct<Loom_MS5803>,			ConstructDefault<Loom_MS5803> },
	// 				Factory::NameModulePair{"SHT31D",		Construct<Loom_SHT31D>,			ConstructDefault<Loom_SHT31D> },
	// 				Factory::NameModulePair{"TMP007",		Construct<Loom_TMP007>,			ConstructDefault<Loom_TMP007> },
	// 				Factory::NameModulePair{"TSL2561",		Construct<Loom_TSL2561>,		ConstructDefault<Loom_TSL2561> },
	// 				Factory::NameModulePair{"TSL2591",		Construct<Loom_TSL2591>,		ConstructDefault<Loom_TSL2591> },
	// 				Factory::NameModulePair{"ZXGesture",	Construct<Loom_ZXGesture>,		ConstructDefault<Loom_ZXGesture> },
			
	// 				Factory::NameModulePair{"Decagon5TM",	Construct<Loom_Decagon5TM>,		ConstructDefault<Loom_Decagon5TM> },
	// 				Factory::NameModulePair{"DecagonGS3",	Construct<Loom_DecagonGS3>,		ConstructDefault<Loom_DecagonGS3> },
			
	// 				Factory::NameModulePair{"MAX31855",		Construct<Loom_MAX31855>,		ConstructDefault<Loom_MAX31855> },
	// 				Factory::NameModulePair{"MAX31856",		Construct<Loom_MAX31856>,		ConstructDefault<Loom_MAX31856> }
	// 			),
	// 			std::make_tuple() // No sensors
	// 		),


	// 		// Actuators
	// 		get<0>(
	// 			std::make_tuple( 
	// 				Factory::NameModulePair{"Neopixel",		Construct<Loom_Neopixel>,		ConstructDefault<Loom_Neopixel> },
	// 				Factory::NameModulePair{"Relay",		Construct<Loom_Relay>,			ConstructDefault<Loom_Relay> },
	// 				Factory::NameModulePair{"Servo",		Construct<Loom_Servo>,			ConstructDefault<Loom_Servo> },
	// 				Factory::NameModulePair{"Stepper",		Construct<Loom_Stepper>,		ConstructDefault<Loom_Stepper> }
	// 			),
	// 			std::make_tuple() // No actuators
	// 		)


	// 	)); // end of LookupTable
















};





















// test

constexpr auto module_list = get<0>(
	std::make_tuple(
			Factory::NameModulePair{"Ethernet",		Construct<Loom_Ethernet_I>,		ConstructDefault<Loom_Ethernet_I> },
			// Factory::NameModulePair{"Ethernet",		Construct<Loom_Ethernet_I>,		nullptr },
			Factory::NameModulePair{"WiFi",			Construct<Loom_WiFi_I>,			ConstructDefault<Loom_WiFi_I> },
			Factory::NameModulePair{"Analog",		Construct<Loom_Analog>,			ConstructDefault<Loom_Analog> }
		),
	std::make_tuple()
);


///////


// currently an issue with single element tuples, resolvable by adding another module (such as a a duplicate)




// From each category, template parameters will select a set of modules to include
// All blocks are then merged for form on array



// Common
// To always be included – can be subdivided further as needed
constexpr auto modules_common = get<0>(
	std::make_tuple( 
			Factory::NameModulePair{"Interrupt_Manager",	Construct<Loom_Interrupt_Manager>,		ConstructDefault<Loom_Interrupt_Manager> },
			Factory::NameModulePair{"Sleep_Manager",		Construct<Loom_Sleep_Manager>,			ConstructDefault<Loom_Sleep_Manager> },
			Factory::NameModulePair{"Analog",				Construct<Loom_Analog>,					ConstructDefault<Loom_Analog> },
			Factory::NameModulePair{"Digital",				Construct<Loom_Digital>,				ConstructDefault<Loom_Digital> },
			// CommPlat
			Factory::NameModulePair{"LoRa",					Construct<Loom_LoRa>,					ConstructDefault<Loom_LoRa> },
			Factory::NameModulePair{"nRF",					Construct<Loom_nRF>,					ConstructDefault<Loom_nRF> },
			Factory::NameModulePair{"Bluetooth",			Construct<Loom_Bluetooth>,				ConstructDefault<Loom_Bluetooth> },
			// PublishPlat
			Factory::NameModulePair{"GoogleSheets",			Construct<Loom_GoogleSheets>,			nullptr },
			Factory::NameModulePair{"MaxPub",				Construct<Loom_MaxPub>,					nullptr },
			// SubscribePlat
			Factory::NameModulePair{"MaxSub",				Construct<Loom_MaxSub>,					nullptr },
			// LogPlat
			Factory::NameModulePair{"OLED",					Construct<Loom_OLED>,					ConstructDefault<Loom_OLED> },
			Factory::NameModulePair{"SD",					Construct<Loom_SD>,						ConstructDefault<Loom_SD> },
			// RTC
			Factory::NameModulePair{"DS3231",				Construct<Loom_DS3231>,					ConstructDefault<Loom_DS3231> },
			Factory::NameModulePair{"PCF8523",				Construct<Loom_PCF8523>,				ConstructDefault<Loom_PCF8523> }
	
		),
	std::make_tuple()
);


// Internet
constexpr auto modules_internet = get<0>(
	std::make_tuple( // Both Ethernet and WiFi
			Factory::NameModulePair{"Ethernet",		Construct<Loom_Ethernet_I>,		ConstructDefault<Loom_Ethernet_I> },
			Factory::NameModulePair{"WiFi",			Construct<Loom_WiFi_I>,			ConstructDefault<Loom_WiFi_I> }
		),
	std::make_tuple( // Only WiFi
			Factory::NameModulePair{"WiFi",			Construct<Loom_WiFi_I>,			ConstructDefault<Loom_WiFi_I> },
			Factory::NameModulePair{"NTP_Sync", 	Construct<LoomNTPSync>,			ConstructDefault<LoomNTPSync> }
		),
	std::make_tuple( // Only Ethernet
			Factory::NameModulePair{"Ethernet",		Construct<Loom_Ethernet_I>,		ConstructDefault<Loom_Ethernet_I> },
			Factory::NameModulePair{"NTP_Sync", 	Construct<LoomNTPSync>,			ConstructDefault<LoomNTPSync> }
		),
	std::make_tuple() // No internet
);


// Sensors
constexpr auto modules_sensors= get<0>(
	std::make_tuple(
			Factory::NameModulePair{"Multiplexer",	Construct<Loom_Multiplexer>,	ConstructDefault<Loom_Multiplexer> },

			Factory::NameModulePair{"AS7262",		Construct<Loom_AS7262>,			ConstructDefault<Loom_AS7262> },
			Factory::NameModulePair{"AS7263",		Construct<Loom_AS7263>,			ConstructDefault<Loom_AS7263> },
			Factory::NameModulePair{"AS7265X",		Construct<Loom_AS7265X>,		ConstructDefault<Loom_AS7265X> },
			Factory::NameModulePair{"FXAS21002",	Construct<Loom_FXAS21002>,		ConstructDefault<Loom_FXAS21002> },
			Factory::NameModulePair{"FXOS8700",		Construct<Loom_FXOS8700>,		ConstructDefault<Loom_FXOS8700> },
			Factory::NameModulePair{"LIS3DH",		Construct<Loom_LIS3DH>,			ConstructDefault<Loom_LIS3DH> },
			Factory::NameModulePair{"MB1232",		Construct<Loom_MB1232>,			ConstructDefault<Loom_MB1232> },
			Factory::NameModulePair{"MMA8451",		Construct<Loom_MMA8451>,		ConstructDefault<Loom_MMA8451> },
			Factory::NameModulePair{"MPU6050",		Construct<Loom_MPU6050>,		ConstructDefault<Loom_MPU6050> },
			Factory::NameModulePair{"MS5803",		Construct<Loom_MS5803>,			ConstructDefault<Loom_MS5803> },
			Factory::NameModulePair{"SHT31D",		Construct<Loom_SHT31D>,			ConstructDefault<Loom_SHT31D> },
			Factory::NameModulePair{"TMP007",		Construct<Loom_TMP007>,			ConstructDefault<Loom_TMP007> },
			Factory::NameModulePair{"TSL2561",		Construct<Loom_TSL2561>,		ConstructDefault<Loom_TSL2561> },
			Factory::NameModulePair{"TSL2591",		Construct<Loom_TSL2591>,		ConstructDefault<Loom_TSL2591> },
			Factory::NameModulePair{"ZXGesture",	Construct<Loom_ZXGesture>,		ConstructDefault<Loom_ZXGesture> },
	
			Factory::NameModulePair{"Decagon5TM",	Construct<Loom_Decagon5TM>,		ConstructDefault<Loom_Decagon5TM> },
			Factory::NameModulePair{"DecagonGS3",	Construct<Loom_DecagonGS3>,		ConstructDefault<Loom_DecagonGS3> },
	
			Factory::NameModulePair{"MAX31855",		Construct<Loom_MAX31855>,		ConstructDefault<Loom_MAX31855> },
			Factory::NameModulePair{"MAX31856",		Construct<Loom_MAX31856>,		ConstructDefault<Loom_MAX31856> }
		),
	std::make_tuple() // No sensors
);


// Actuators
constexpr auto modules_actuators = get<0>(
	std::make_tuple( 
			Factory::NameModulePair{"Neopixel",		Construct<Loom_Neopixel>,		ConstructDefault<Loom_Neopixel> },
			Factory::NameModulePair{"Relay",		Construct<Loom_Relay>,			ConstructDefault<Loom_Relay> },
			Factory::NameModulePair{"Servo",		Construct<Loom_Servo>,			ConstructDefault<Loom_Servo> },
			Factory::NameModulePair{"Stepper",		Construct<Loom_Stepper>,		ConstructDefault<Loom_Stepper> }
		),
	std::make_tuple() // No actuators
);







