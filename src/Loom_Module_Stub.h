
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





class Loom_Module_Stub : public LoomModule
{

private: 
	const char* error_msg = "No valid module found";

public:
	Loom_Module_Stub() {};
	~Loom_Module_Stub() {};
	
	// Loom Module
	ModuleType		get_module_type()
	{ return ModuleType::Unknown; }
	LoomManager*	get_device_manager()
	{ return nullptr; }
	void link_device_manager(LoomManager* LM) 
	{ Println(error_msg); }
	void print_module_label() 
	{ Println("[Global Stub class]"); }
	void print_config() 
	{ Println(error_msg); }
	void print_state() 
	{ Println(error_msg); }

	void measure()
	{ Println(error_msg); }
	void package(OSCBundle& bndl, char* suffix="") 
	{ Println(error_msg); }
	bool message_route(OSCMessage& msg, int address_offset) 
	{ Println(error_msg); return false; }

	void get_module_name(char* buf)
	{ Println(error_msg); }
	const char*	get_module_name()
	{ Println(error_msg); return "None"; }
	void set_print_verbosity(Verbosity v)
	{ Println(error_msg); }
	Verbosity get_print_verbosity()
	{ Println(error_msg); return Verbosity::V_OFF; }
	void set_package_verbosity(Verbosity v)
	{ Println(error_msg); }
	Verbosity get_package_verbosity()
	{ Println(error_msg); return Verbosity::V_OFF; }
	void set_active(bool enable)
	{ Println(error_msg); }
	bool get_active()
	{ Println(error_msg); }
	void set_print_debug(bool enable)
	{ Println(error_msg); }
	bool get_print_debug()
	{ Println(error_msg); }
	void power_down()
	{ Println(error_msg); }
	void power_up()
	{ Println(error_msg); }
	void save_config()	
	{ Println(error_msg); }
	bool load_config()
	{ Println(error_msg); }
	void print_config_struct()
	{ Println(error_msg); }
	void REPL_loop()
	{ Println(error_msg); }




	/////////////////////////////////////////////////////////////////////
	// Sleep Manager

	void set_RTC_module(LoomRTC* RTC_Inst)
	{ Println(error_msg); }
	LoomRTC* get_RTC_module()
	{ Println(error_msg); }
	void set_Interrupt_Manager(Loom_Interrupt_Manager* IM)
	{ Println(error_msg); }
	Loom_Interrupt_Manager*	get_Interrupt_Manager()
	{ Println(error_msg); }
	void set_sleep_mode(SleepMode mode)
	{ Println(error_msg); }
	SleepMode get_sleep_mode()
	{ Println(error_msg); }
	bool sleep_duration(TimeSpan duration, bool from_last_wake=false)
	{ Println(error_msg); }
	bool sleep_duration(uint days, uint hours, uint minutes, uint seconds, bool from_last_wake=false)
	{ Println(error_msg); }
	bool sleep_until(DateTime future_time)
	{ Println(error_msg); }
	bool sleep_until(uint hour, uint minute, uint second)
	{ Println(error_msg); }
	bool sleep()
	{ Println(error_msg); return false; }


	// Interrupt Manager
	void run_pending_ISRs()
	{ Println(error_msg); }
	void set_interrupts_enabled(bool state)
	{ Println(error_msg); }
	bool get_interrupts_enabled()
	{ Println(error_msg); }
	void set_enable_interrupt(byte pin, bool state)
	{ Println(error_msg); }
	bool get_enable_interrupt(byte pin)
	{ Println(error_msg); }
	void register_ISR(byte pin, ISRFuncPtr ISR, byte type, bool immediate)
	{ Println(error_msg); }
	void unregister_ISR(byte pin, byte type=LOW)
	{ Println(error_msg); }
	void run_ISR_bottom_halves()
	{ Println(error_msg); }
	void interrupt_reset(byte pin)
	{ Println(error_msg); }
	bool RTC_alarm_relative(TimeSpan duration)
	{ Println(error_msg); }
	bool RTC_alarm_relative(uint days, uint hours, uint minutes, uint seconds)
	{ Println(error_msg); }
	bool RTC_alarm_exact(DateTime future_time)
	{ Println(error_msg); }
	bool RTC_alarm_exact(uint hour, uint minute, uint second)
	{ Println(error_msg); }
	void check_timers()
	{ Println(error_msg); }
	void register_timer(uint timer_num, unsigned long duration, ISRFuncPtr ISR, bool repeat)
	{ Println(error_msg); }
	void clear_timer(uint timer_num)
	{ Println(error_msg); }




	/////////////////////////////////////////////////////////////////////
	// Communiciation Platforms

	bool receive_bundle(OSCBundle& bndl)
	{ Println(error_msg); }
	bool send_bundle(OSCBundle& bndl)
	{ Println(error_msg); }
	void broadcast_bundle(OSCBundle& bndl)
	{ Println(error_msg); }
	void set_address(uint a)
	{ Println(error_msg); }
	uint get_address()
	{ Println(error_msg); }
	void set_compress_messages(bool c)
	{ Println(error_msg); }
	bool get_compress_messages()
	{ Println(error_msg); }
	bool scope_filter(OSCBundle& bndl, int* offset)
	{ Println(error_msg); }
	bool scope_filter(OSCBundle& bndl)
	{ Println(error_msg); }
	void set_friend_address(uint addr)
	{ Println(error_msg); }
	uint get_friend_address()
	{ Println(error_msg); }

	// LoRa

	// nRF 
	void set_multicast_level(uint8_t level)
	{ Println(error_msg); }
	uint8_t get_multicast_level()
	{ Println(error_msg); }

	// SlipSerial
	



	/////////////////////////////////////////////////////////////////////
	// Logging Platforms
	void log_bundle(OSCBundle& bndl)
	{ Println(error_msg); }

	// OLED
	void log_bundle(OSCBundle& bndl, OLED_Format format)
	{ Println(error_msg); }
	void set_display_format(OLED_Format format)
	{ Println(error_msg); }
	OLED_Format	get_display_format()
	{ Println(error_msg); }
	void set_scroll_duration(uint duration)
	{ Println(error_msg); }
	uint get_scroll_duration()
	{ Println(error_msg); }
	void set_freeze_pin(byte pin)
	{ Println(error_msg); }
	byte get_freeze_pin()
	{ Println(error_msg); }
	void set_freeze_behavior(OLED_Freeze behavior)
	{ Println(error_msg); }
	OLED_Freeze	get_freeze_behavior()
	{ Println(error_msg); }


	// SD
	bool save_bundle(OSCBundle& bndl, char* file, int timestamp=3)
	{ Println(error_msg); }
	void set_default_file(char* filename)
	{ Println(error_msg); }
	char* get_default_file()
	{ Println(error_msg); }
	void delete_file(char* file)
	{ Println(error_msg); }
	void empty_file(char* file)
	{ Println(error_msg); }
	void list_files()
	{ Println(error_msg); }
	bool dump_file(char* file) 
	{ Println(error_msg); }
	template <typename T> bool save_array(char *file, T data [], int len, char delimiter=',', int timestamp=3, bool has_keys=false, char* device_id="")
	{ Println(error_msg); }


	/////////////////////////////////////////////////////////////////////
	// Internet Platforms



	/////////////////////////////////////////////////////////////////////
	// Mulitplexer
	void get_sensor_list(OSCBundle& bndl)
	{ Println(error_msg); }
	void set_is_dynamic(bool dynamic)
	{ Println(error_msg); }
	bool get_is_dynamic()
	{ Println(error_msg); }
	void set_update_period(int period)
	{ Println(error_msg); }
	int get_update_period()
	{ Println(error_msg); }
	void refresh_sensors()
	{ Println(error_msg); }


	/////////////////////////////////////////////////////////////////////
	// Sensors
	void print_measurements()
	{ Println(error_msg); } 
	void calibrate()
	{ Println(error_msg); }
	void set_num_samples(uint8_t n)
	{ Println(error_msg); }
	uint8_t get_num_samples()
	{ Println(error_msg); }


	// Analog
	void set_analog_resolution(uint8_t res)
	{ Println(error_msg); }
	uint8_t get_analog_resolution()
	{ Println(error_msg); }
	int get_analog_val(uint8_t pin)
	{ Println(error_msg); }
	float get_battery()
	{ Println(error_msg); }
	bool get_pin_enabled(uint8_t pin)
	{ Println(error_msg); }	
	void set_pin_enabled(uint8_t pin, bool e)
	{ Println(error_msg); }
	AnalogConversion get_conversion(uint8_t pin)
	{ Println(error_msg); }
	void set_conversion(uint8_t pin, AnalogConversion c)
	{ Println(error_msg); }
	void set_enable_conversions(bool e)
	{ Println(error_msg); }
	float convert(uint8_t pin, uint16_t analog)
	{ Println(error_msg); }


	// Digital
	bool get_digital_val(uint8_t pin)
	{ Println(error_msg); }
	void set_digital_val(uint8_t pin, bool state)
	{ Println(error_msg); }
	



	/////////////////////////////////////////////////////////////////////
	// I2C Sensors
	byte get_i2c_address()
	{ Println(error_msg); } 	


	// Loom_AS7262 / Loom_AS7263 / Loom_AS7265X
	void enable_bulb(bool enable)
	{ Println(error_msg); }
	void set_gain(byte gain)
	{ Println(error_msg); }
	void set_mode(byte mode)
	{ Println(error_msg); }
	void set_integration_time(byte time)
	{ Println(error_msg); }


	/////////////////////////////////////////////////////////////////////
	// SDI12 Sensors

	/////////////////////////////////////////////////////////////////////
	// SPI Sensors




	/////////////////////////////////////////////////////////////////////
	// RTC

	DateTime now()
	{ Println(error_msg); }
	void print_DateTime(DateTime time)
	{ Println(error_msg); }
	void print_time(bool verbose=false)
	{ Println(error_msg); }
	char* get_datestring()
	{ Println(error_msg); } 
	void get_datestring(char* buf)
	{ Println(error_msg); } 
	char* get_timestring()
	{ Println(error_msg); }
	void get_timestring(char* buf)
	{ Println(error_msg); }
	char* get_weekday()
	{ Println(error_msg); }
	void get_weekday(char* buf)
	{ Println(error_msg); }
	void get_timestamp(char* header, char* timestamp, char delimiter, uint8_t format=3)
	{ Println(error_msg); }
	void set_alarm(DateTime time)
	{ Println(error_msg); }
	void set_alarm(TimeSpan duration)
	{ Println(error_msg); }
	void clear_alarms()
	{ Println(error_msg); }
	byte get_interrupt_pin()
	{ Println(error_msg); }
	void RTC_Wake_ISR()
	{ Println(error_msg); }


	/////////////////////////////////////////////////////////////////////
	// Actuators


	// Neopixel
	void enable_pin(uint8_t port, bool state)
	{ Println(error_msg); }
	void set_color(uint8_t port, uint8_t chain_num, uint8_t red, uint8_t green, uint8_t blue)
	{ Println(error_msg); }
	void set_color(OSCMessage& msg)
	{ Println(error_msg); }

	// Relay
	void set_relay(bool state)
	{ Println(error_msg); }
	void set_relay(OSCMessage& msg)
	{ Println(error_msg); }

	// Servo
	void set_degree(int servo, int degree)
	{ Println(error_msg); }
	void set_degree(OSCMessage& msg)
	{ Println(error_msg); }

	// Stepper
	void move_steps(int motor, int steps, int speed, bool clockwise)
	{ Println(error_msg); }
	void move_steps(OSCMessage& msg)
	{ Println(error_msg); }












};

