
#ifndef LOOM_INTERRUPT_MANAGER_h
#define LOOM_INTERRUPT_MANAGER_h

#include "Loom_Module.h"


#include <RTClibExtended.h>
// NOTE: Must include the following line in the RTClibExtended.h file to use with M0:
// #define _BV(bit) (1 << (bit))


#define EI_NOTEXTERNAL
#include <EnableInterrupt.h>


class LoomRTC; // Specify that LoomRTC exists, defined in own file



// Used to make function signatures easier to read 
// when returning function pointers
typedef void (*ISRFuncPtr)(); 



// INT_LOW		: 0
// INT_HIGH		: 1
// INT_CHANGE 	: 2
// INT_FALLING	: 3
// INT_RISING	: 4
// enum IntType { INT_LOW, INT_HIGH, INT_CHANGE, INT_FALLING, INT_RISING };


struct IntDetails {
	ISRFuncPtr 	ISR;			// set null if no interrupt linked
	byte	 	type;
	bool 		is_immediate;
	bool 		is_enabled;

	// char[20] description ?
};


#define InteruptRange 16


class Loom_Interrupt_Manager : public LoomModule
{

protected:

	// Enable or disable all interrupts 	-- currently only disables bottom halves
	bool 			interrupts_enabled;

	IntDetails		settings[InteruptRange];

	// Flags set by interrupts, indicating ISR bottom 
	// half should be called if not Null
	static bool 	interrupt_triggered[InteruptRange];


	LoomRTC*		RTC_Inst;

	DateTime 	last_alarm_time;


	// interrupt_triggered for timers, also support immediate and delayed
public:

	// --- CONSTRUCTOR ---
	Loom_Interrupt_Manager( char* 		module_name 	= "Interrupt_Manager",
		
							LoomRTC* 	RTC_Inst		= NULL
							);


	// --- DESTRUCTOR ---
	~Loom_Interrupt_Manager();

	// General
	void print_config();
	void print_state();
	void measure() {}
	void package(OSCBundle& bndl, char* suffix="") {}
	bool message_route(OSCMessage& msg, int address_offset) {}

	// All interrupt enable
	void set_interrupts_enabled(bool state);
	bool get_interrupts_enabled();

	// Per interrupt enable
	void set_enable_interrupt(byte pin, bool state);
	bool get_enable_interrupt(byte pin);


	// Pin: which pin to connect the interrupt on
	// ISR: ISR function (Null if no interrupt linked)
	// Type: Low, High, Change, Falling, Rising
	// Immediate: Whether the interrupt runs immediately, else sets flag to check and runs ISR when flag checked
	void register_ISR(byte pin, ISRFuncPtr ISR, byte type, bool immediate);

	// Restores to default ISR, disables interrupt
	void unregister_ISR(byte pin, byte type=LOW);  

	// Checks the flags set by default ISRs, to call bottom half ISRs
	void run_ISR_bottom_halves();


	// Detaches then reattacheds interrupt according to settings
	// used to clear pending interrupts
	void interrupt_reset(byte pin);   


	void set_RTC_module(LoomRTC* RTC_Inst);
	LoomRTC* get_RTC_module();


// Shorten names, maybe combine, taking behavior (e.g. relative/absolute) as parameter

	bool set_RTC_alarm_time_into_future(TimeSpan duration);	
	bool set_RTC_alarm_time_into_future(uint days, uint hours, uint minutes, uint seconds);
	
// maybe remove these 2 (leave in Sleep manager)
	// bool set_RTC_alarm_for_time_from_last_alarm_time(TimeSpan duration);
	// bool set_RTC_alarm_for_time_from_last_alarm_time(uint days, uint hours, uint minutes, uint seconds);
	
	bool set_RTC_alarm_for_time(DateTime future_time);
	bool set_RTC_alarm_for_time(uint hour, uint minute, uint second);
	



	// Set timer for time / duration, select which timer (1 or 2)


// set_timer_interrupt_for()   ?


private:


	// Default ISRs that set flags, detach interrupt to prevent multiple triggering (reattached after checking flag)
	static void default_ISR_0()  { interrupt_triggered[0]  = true; };
	static void default_ISR_1()  { interrupt_triggered[1]  = true; };
	static void default_ISR_2()  { interrupt_triggered[2]  = true; };
	static void default_ISR_3()  { interrupt_triggered[3]  = true; };
	static void default_ISR_4()  { interrupt_triggered[4]  = true; };
	static void default_ISR_5()  { interrupt_triggered[5]  = true; };
	static void default_ISR_6()  { interrupt_triggered[6]  = true; };
	static void default_ISR_7()  { interrupt_triggered[7]  = true; };
	static void default_ISR_8()  { interrupt_triggered[8]  = true; };
	static void default_ISR_9()  { interrupt_triggered[9]  = true; };
	static void default_ISR_10() { interrupt_triggered[10] = true; };
	static void default_ISR_11() { interrupt_triggered[11] = true; };
	static void default_ISR_12() { interrupt_triggered[12] = true; };
	static void default_ISR_13() { interrupt_triggered[13] = true; };
	static void default_ISR_14() { interrupt_triggered[14] = true; };
	static void default_ISR_15() { interrupt_triggered[15] = true; };

	// static void default_ISR_0()  { detachInterrupt(digitalPinToInterrupt(0));  interrupt_triggered[0]  = true;   }
	// static void default_ISR_1()  { detachInterrupt(digitalPinToInterrupt(1));  interrupt_triggered[1]  = true;   }
	// static void default_ISR_2()  { detachInterrupt(digitalPinToInterrupt(2));  interrupt_triggered[2]  = true;   }
	// static void default_ISR_3()  { detachInterrupt(digitalPinToInterrupt(3));  interrupt_triggered[3]  = true;   }
	// static void default_ISR_4()  { detachInterrupt(digitalPinToInterrupt(4));  interrupt_triggered[4]  = true;   }
	// static void default_ISR_5()  { detachInterrupt(digitalPinToInterrupt(5));  interrupt_triggered[5]  = true;   }
	// static void default_ISR_6()  { detachInterrupt(digitalPinToInterrupt(6));  interrupt_triggered[6]  = true;   }
	// static void default_ISR_7()  { detachInterrupt(digitalPinToInterrupt(7));  interrupt_triggered[7]  = true;   }
	// static void default_ISR_8()  { detachInterrupt(digitalPinToInterrupt(8));  interrupt_triggered[8]  = true;   }
	// static void default_ISR_9()  { detachInterrupt(digitalPinToInterrupt(9));  interrupt_triggered[9]  = true;   }
	// static void default_ISR_10() { detachInterrupt(digitalPinToInterrupt(10));  interrupt_triggered[10] = true;   }
	// static void default_ISR_11() { detachInterrupt(digitalPinToInterrupt(11));  interrupt_triggered[11] = true;   }
	// static void default_ISR_12() { detachInterrupt(digitalPinToInterrupt(12));  interrupt_triggered[12] = true;   }
	// static void default_ISR_13() { detachInterrupt(digitalPinToInterrupt(13));  interrupt_triggered[13] = true;   }
	// static void default_ISR_14() { detachInterrupt(digitalPinToInterrupt(14));  interrupt_triggered[14] = true;   }
	// static void default_ISR_15() { detachInterrupt(digitalPinToInterrupt(15));  interrupt_triggered[15] = true;   }

	const static ISRFuncPtr default_ISRs[InteruptRange];

};


#endif // of #ifndef LOOM_INTERRUPT_MANAGER_h












