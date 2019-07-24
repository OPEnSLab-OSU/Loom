#pragma once

#include "Loom_Module.h"

// For OLED enums
#include "LogPlats/Loom_OLED.h"
// For Analog Enums
#include "Sensors/Loom_Analog.h"
// For DateTime/TimeSpan objects
#include "RTC/Loom_RTC.h"
// For Sleep Enums
#include "Loom_Sleep_Manager.h"
// For Interrupt Enums
#include "Loom_Interrupt_Manager.h"



/// NOTE: This module is broken, as it attempts to implement member functions
/// of classes it does not inherit from. If you need to check if you
/// recieved a stub module, check the modules type using
/// (LoomModule*)(module)->get_module_type().

class Loom_Module_Stub : public LoomModule
{

private: 

	const char* error_msg = "No valid module found";

public:
	Loom_Module_Stub() {};
	~Loom_Module_Stub() {};
	
	/// \cond DO_NOT_DOCUMENT

	// Loom Module
	LoomModule::Type get_module_type()
	{ return LoomModule::Type::Unknown; }
	LoomManager*	get_device_manager()
	{ return nullptr; }
	void link_device_manager(LoomManager* LM) 
	{ LPrintln(error_msg); }
	void print_module_label() 
	{ LPrintln("[Global Stub class]"); }
	void print_config() 
	{ LPrintln(error_msg); }
	void print_state() 
	{ LPrintln(error_msg); }

	void measure()
	{ LPrintln(error_msg); }
	void package(JsonObject json) 
	{ LPrintln(error_msg); }
	bool dispatch(JsonObject) 
	{ LPrintln(error_msg); return false; }

	void get_module_name(char* buf)
	{ LPrintln(error_msg); }
	const char*	get_module_name()
	{ LPrintln(error_msg); return "None"; }
	void set_print_verbosity(Verbosity v)
	{ LPrintln(error_msg); }
	Verbosity get_print_verbosity()
	{ LPrintln(error_msg); return Verbosity::V_OFF; }
	void set_package_verbosity(Verbosity v)
	{ LPrintln(error_msg); }
	Verbosity get_package_verbosity()
	{ LPrintln(error_msg); return Verbosity::V_OFF; }
	void set_active(bool enable)
	{ LPrintln(error_msg); }
	bool get_active()
	{ LPrintln(error_msg); }
	void set_print_debug(bool enable)
	{ LPrintln(error_msg); }
	bool get_print_debug()
	{ LPrintln(error_msg); }
	void power_down()
	{ LPrintln(error_msg); }
	void power_up()
	{ LPrintln(error_msg); }
	void save_config()	
	{ LPrintln(error_msg); }
	bool load_config()
	{ LPrintln(error_msg); }
	void print_config_struct()
	{ LPrintln(error_msg); }


	///////////////////////////////////////////////////////////////////////////////
	// Sleep Manager

	void set_RTC_module(LoomRTC* RTC_Inst)
	{ LPrintln(error_msg); }
	LoomRTC* get_RTC_module()
	{ LPrintln(error_msg); }
	void set_Interrupt_Manager(Loom_Interrupt_Manager* IM)
	{ LPrintln(error_msg); }
	Loom_Interrupt_Manager*	get_Interrupt_Manager()
	{ LPrintln(error_msg); }
	void set_sleep_mode(Loom_Sleep_Manager::Mode mode)
	{ LPrintln(error_msg); }
	Loom_Sleep_Manager::Mode get_sleep_mode()
	{ LPrintln(error_msg); }
	bool sleep_duration(TimeSpan duration, bool from_last_wake=false)
	{ LPrintln(error_msg); }
	bool sleep_duration(uint days, uint hours, uint minutes, uint seconds, bool from_last_wake=false)
	{ LPrintln(error_msg); }
	bool sleep_until(DateTime future_time)
	{ LPrintln(error_msg); }
	bool sleep_until(uint hour, uint minute, uint second)
	{ LPrintln(error_msg); }
	bool sleep()
	{ LPrintln(error_msg); return false; }


	// Interrupt Manager
	void run_pending_ISRs()
	{ LPrintln(error_msg); }
	void set_interrupts_enabled(bool state)
	{ LPrintln(error_msg); }
	bool get_interrupts_enabled()
	{ LPrintln(error_msg); }
	void set_enable_interrupt(byte pin, bool state)
	{ LPrintln(error_msg); }
	bool get_enable_interrupt(byte pin)
	{ LPrintln(error_msg); }
	void register_ISR(byte pin, ISRFuncPtr ISR, byte type, bool immediate)
	{ LPrintln(error_msg); }
	void unregister_ISR(byte pin, byte type=LOW)
	{ LPrintln(error_msg); }
	void run_ISR_bottom_halves()
	{ LPrintln(error_msg); }
	void interrupt_reset(byte pin)
	{ LPrintln(error_msg); }
	bool RTC_alarm_relative(TimeSpan duration)
	{ LPrintln(error_msg); }
	bool RTC_alarm_relative(uint days, uint hours, uint minutes, uint seconds)
	{ LPrintln(error_msg); }
	bool RTC_alarm_exact(DateTime future_time)
	{ LPrintln(error_msg); }
	bool RTC_alarm_exact(uint hour, uint minute, uint second)
	{ LPrintln(error_msg); }
	void check_timers()
	{ LPrintln(error_msg); }
	void register_timer(uint timer_num, unsigned long duration, ISRFuncPtr ISR, bool repeat)
	{ LPrintln(error_msg); }
	void clear_timer(uint timer_num)
	{ LPrintln(error_msg); }


	///////////////////////////////////////////////////////////////////////////////
	// Communiciation Platforms

	bool receive(JsonObject json)
	{ LPrintln(error_msg); }
	bool send(JsonObject json)
	{ LPrintln(error_msg); }
	bool send()
	{ LPrintln(error_msg); }
	void broadcast(JsonObject json)
	{ LPrintln(error_msg); }
	void set_address(uint a)
	{ LPrintln(error_msg); }
	uint get_address()
	{ LPrintln(error_msg); }
	void set_friend_address(uint addr)
	{ LPrintln(error_msg); }
	uint get_friend_address()
	{ LPrintln(error_msg); }

	// LoRa

	// nRF 
	void set_multicast_level(uint8_t level)
	{ LPrintln(error_msg); }
	uint8_t get_multicast_level()
	{ LPrintln(error_msg); }

	// SlipSerial
	

	///////////////////////////////////////////////////////////////////////////////
	// Logging Platforms
	void log()
	{ LPrintln(error_msg); }
	void log(JsonObject json)
	{ LPrintln(error_msg); }

	// OLED
	void set_display_format(Loom_OLED::Format format)
	{ LPrintln(error_msg); }
	Loom_OLED::Format get_display_format()
	{ LPrintln(error_msg); }
	void set_scroll_duration(uint duration)
	{ LPrintln(error_msg); }
	uint get_scroll_duration()
	{ LPrintln(error_msg); }
	void set_freeze_pin(byte pin)
	{ LPrintln(error_msg); }
	byte get_freeze_pin()
	{ LPrintln(error_msg); }
	void set_freeze_behavior(Loom_OLED::FreezeType behavior)
	{ LPrintln(error_msg); }
	Loom_OLED::FreezeType get_freeze_behavior()
	{ LPrintln(error_msg); }


	// SD
	void log(const char* filename)
	{ LPrintln(error_msg); }
	bool save_json(JsonObject json, char* file, int timestamp=3)
	{ LPrintln(error_msg); }
	void set_default_file(char* filename)
	{ LPrintln(error_msg); }
	char* get_default_file()
	{ LPrintln(error_msg); }
	void delete_file(char* file)
	{ LPrintln(error_msg); }
	void empty_file(char* file)
	{ LPrintln(error_msg); }
	void list_files()
	{ LPrintln(error_msg); }
	bool dump_file(char* file) 
	{ LPrintln(error_msg); }
	template <typename T> bool save_array(char *file, T data [], int len, char delimiter=',', int timestamp=3, bool has_keys=false, char* device_id="")
	{ LPrintln(error_msg); }


	///////////////////////////////////////////////////////////////////////////////
	// Internet Platforms
	void connect()
	{ LPrintln(error_msg); }
	bool is_connected()
	{ LPrintln(error_msg); return false; }
	uint32_t get_time()
	{ LPrintln(error_msg); return 0; }



	///////////////////////////////////////////////////////////////////////////////
	// Mulitplexer
	void get_sensor_list(JsonObject json)
	{ LPrintln(error_msg); }
	void set_is_dynamic(bool dynamic)
	{ LPrintln(error_msg); }
	bool get_is_dynamic()
	{ LPrintln(error_msg); }
	void set_update_period(int period)
	{ LPrintln(error_msg); }
	int get_update_period()
	{ LPrintln(error_msg); }
	void refresh_sensors()
	{ LPrintln(error_msg); }


	///////////////////////////////////////////////////////////////////////////////
	// Sensors
	void print_measurements()
	{ LPrintln(error_msg); } 
	void calibrate()
	{ LPrintln(error_msg); }
	void set_num_samples(uint8_t n)
	{ LPrintln(error_msg); }
	uint8_t get_num_samples()
	{ LPrintln(error_msg); }


	// Analog
	void set_analog_resolution(uint8_t res)
	{ LPrintln(error_msg); }
	uint8_t get_analog_resolution()
	{ LPrintln(error_msg); }
	int get_analog_val(uint8_t pin)
	{ LPrintln(error_msg); }
	float get_battery()
	{ LPrintln(error_msg); }
	bool get_pin_enabled(uint8_t pin)
	{ LPrintln(error_msg); }	
	void set_pin_enabled(uint8_t pin, bool e)
	{ LPrintln(error_msg); }
	Loom_Analog::Conversion get_conversion(uint8_t pin)
	{ LPrintln(error_msg); }
	void set_conversion(uint8_t pin, Loom_Analog::Conversion c)
	{ LPrintln(error_msg); }
	void set_enable_conversions(bool e)
	{ LPrintln(error_msg); }
	float convert(uint8_t pin, uint16_t analog)
	{ LPrintln(error_msg); }


	// Digital
	bool get_digital_val(uint8_t pin)
	{ LPrintln(error_msg); }
	void set_digital_val(uint8_t pin, bool state)
	{ LPrintln(error_msg); }
	

	///////////////////////////////////////////////////////////////////////////////
	// I2C Sensors
	byte get_i2c_address()
	{ LPrintln(error_msg); } 	


	// Loom_AS7262 / Loom_AS7263 / Loom_AS7265X
	void enable_bulb(bool enable)
	{ LPrintln(error_msg); }
	void set_gain(byte gain)
	{ LPrintln(error_msg); }
	void set_mode(byte mode)
	{ LPrintln(error_msg); }
	void set_integration_time(byte time)
	{ LPrintln(error_msg); }


	///////////////////////////////////////////////////////////////////////////////
	// SDI12 Sensors

	///////////////////////////////////////////////////////////////////////////////
	// SPI Sensors




	///////////////////////////////////////////////////////////////////////////////
	// RTC

	DateTime now()
	{ LPrintln(error_msg); }
	void print_DateTime(DateTime time)
	{ LPrintln(error_msg); }
	void print_time(bool verbose=false)
	{ LPrintln(error_msg); }
	char* get_datestring()
	{ LPrintln(error_msg); } 
	void get_datestring(char* buf)
	{ LPrintln(error_msg); } 
	char* get_timestring()
	{ LPrintln(error_msg); }
	void get_timestring(char* buf)
	{ LPrintln(error_msg); }
	char* get_weekday()
	{ LPrintln(error_msg); }
	void get_weekday(char* buf)
	{ LPrintln(error_msg); }
	void get_timestamp(char* header, char* timestamp, char delimiter, uint8_t format=3)
	{ LPrintln(error_msg); }
	void set_alarm(DateTime time)
	{ LPrintln(error_msg); }
	void set_alarm(TimeSpan duration)
	{ LPrintln(error_msg); }
	void clear_alarms()
	{ LPrintln(error_msg); }
	byte get_interrupt_pin()
	{ LPrintln(error_msg); }
	void RTC_Wake_ISR()
	{ LPrintln(error_msg); }


	///////////////////////////////////////////////////////////////////////////////
	// Actuators


	// Neopixel
	void enable_pin(uint8_t port, bool state)
	{ LPrintln(error_msg); }
	void set_color(uint8_t port, uint8_t chain_num, uint8_t red, uint8_t green, uint8_t blue)
	{ LPrintln(error_msg); }

	// Relay
	void set(bool state)
	{ LPrintln(error_msg); }

	// Servo
	void set_degree(int servo, int degree)
	{ LPrintln(error_msg); }

	// Stepper
	void move_steps(int motor, int steps, int speed, bool clockwise)
	{ LPrintln(error_msg); }

	/// \endcond

};

