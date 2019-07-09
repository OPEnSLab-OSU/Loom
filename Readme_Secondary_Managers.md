

# Secondary Managers

This guide details the typical usage of Loom's secondary managers.

## Table of Contents

- [Interrupt Manager](#interrupt-manager)
- [Sleep Manager](#sleep-manager)
- [Multiplexer](#multiplexer)

## Interrupt Manager

The `Loom_Interrupt_Manager` is used to manage interrupts of a variety of types (e.g. external, RTC alarm, internal timer, interal stopwatch).

#### Interrupt Signal Types

- HIGH
- LOW
- RISING
- FALLING
- CHANGE (rising or falling)

*Note:* If the device is asleep, only HIGH and LOW can be detected.

### ISRS

Interrupts need to be associated with an interrupt service routines (ISR). An ISR is a basic function that runs immediately when the interrupt happens, temporarily jumping from whatever the program was currently executing, before return to what it was doing. As such, you want ot keep your ISRs as simple as possible.

Things to generally avoid in ISRs (see [Check Flag ISRs](#check-flag) if you want to use the following):

- Serial prints
- Delays
- Function calls

#### ISR Types

The interrupt manager supports two types of ISRs, immediate and flag-based,  the difference lying in when they are run. 

**Immediate:** The ISR will run in its entirety immediately upon the interrupt triggering

**Check Flag:** When the interrupt triggers, a default ISR will set a flag indicating that the ISR you provided should be run when `run_pending_ISRs()` is called (your ISR is known as an ISR bottom half). Use this if the execution of your ISR can wait until the next iteration of `loop()`; if it is relatively long; or if you  want to use commands like Serial prints, delay, or function calls. These ISRs are similar to setting a flag in an immediate ISR and running code in an `if (flag) { … }`, either approach is acceptable.

### Example ISRs

In the case of a basic external interrupt (just a HIGH or LOW signal), the ISR does not need any content, the following would suffice:

#### Sleep

```cpp
void wakeISR() {}
```

However, you may want to detach the interrupt to prevent it from triggering a few times (though that would not impact program behavior)

```cpp
void wakeISR() {
  detachInterrupt(digitalPinToInterrupt(WAKE_PIN)); 
  // where WAKE_PIN is the pin the interrupt is connected to
}
```

#### RTC

If your interrupt comes from an RTC, you need to detach the interrupt to prevent it from triggering repeatedly (as the RTC outputs a squarewave to the interrupt pin),  essentially freezing the program 

```cpp
void wakeISR_RTC() {
  detachInterrupt(digitalPinToInterrupt(RTC_PIN)); 
  // where RTC_PIN is the pin the interrupt is connected to
}
```

#### Multiple Interrupts

If you have multiple sources of interrupts you will likely want to use flags (boolean variables) in the ISRs so that the rest of the program can be behave according to which interrupt fired.

For example, if you had both an RTC and a button that could trigger interrupts and the behavior to respond to each is different, you might use the following:

```cpp
volatile alarmFlag = false;
void RTC_ISR() { 
	detachInterrupt(digitalPinToInterrupt(RTC_PIN)); 
	alarmFlag = true;
}

volatile buttonFlag = false;
void buttonISR() { 
	detachInterrupt(digitalPinToInterrupt(BUTTON_PIN));
	buttonFlag = true;
}

setup() 
{
  ...
  // register interrupts 
  ...
}

loop() 
{
  ...
	if (alarmFlag) {
   	...
 		// Handle wake from alarm functionality
    ...
    alarmFlag = false;
    // reconnect alarm interrupt
  }
  if (buttonFlag) {
    ...
 		// Handle wake from button functionality
    ...
    buttonFlag = false;
    // reconnect button interrupt
  }
  ...
}
```

If all the interrupts are intended to trigger the same behavior, the flags are not always necessary.

### General Operation

These methods are used for most associating ISRs with interrupts, and various common functionality. External interrupts can be implemented completely with the following. RTC and other interrupt will also make use of these methods.

#### Register ISR

Register an ISR to an interrupt pin and its configuration.

`void register_ISR(byte pin, ISRFuncPtr ISR, byte signal_type, ISR_Type run_type);`

**pin:** Which pin to connect the interrupt on
**ISR:** ISR function (Null if no interrupt linked)
**signal_type:** Low, High, Change, Falling, Rising
**run_type:** Whether the interrupt runs immediately, else sets flag to check and runs ISR when flag checked

**Note: ** If your ISR detaches the interrupt with `detachInterrupt` you will need to re-register the ISR for it to be triggered again. 

**Example:**

This is not a complete example, intended only to show how you would register an ISR with an interrupt.

```cpp
void alarmISR() { 
	detachInterrupt(digitalPinToInterrupt(ALARM_PIN)); 
	alarmFlag = true;
}

void buttonISR() { 
	buttonFlag = true;
}

...

setup() {
  ...
  Manager.InterruptManager().register_ISR(6, alarmISR, LOW, ISR_Type::IMMEDIATE);
  Manager.InterruptManager().register_ISR(10, buttonISR, HIGH, ISR_Type::CHECK_FLAG);
  ...
}
```

#### Run Pending ISRs

Run any waiting ISRs. This applys to bottom half ISRs (those specified as `ISR_Type::CHECK_FLAG`). If you don't call this method, any `CHECK_FLAG` ISRs will not run. It also applies to some timer ISRs.

```cpp
Manager.InterruptManager().run_pending_ISRs();
```

#### Unregister ISR

Restores pin to default ISR, disables interrupt.

```cpp
// Remove ISR associated with pin 6
Manager.InterruptManager().unregister(6); 
```

#### Interrupt Reset

Detaches then reattacheds interrupt according to settings. Used to clear pending interrupts.

```cpp
// Reset interrupt pin 6
Manager.InterruptManager().interrupt_reset(6);
```

### RTC Alarm Methods

Use these methods to set RTC alarms. Requires RTC hardware connected and a `LoomRTC` module linked (done automatically if using `LoomManager`, or `set_RTC_module(LoomRTC*)` if manual).

#### RTC Alarm Duration

Set RTC alarm an amount of time from now.

`bool RTC_alarm_duration(TimeSpan duration);` 

`bool RTC_alarm_duration(uint days, uint hours, uint minutes, uint seconds);`

Example:

```cpp
// duration of 300 seconds with TimeSpan
InterruptManager().RTC_alarm_duration(TimeSpan(300));
// or
// duration of 5 minutes
InterruptManager().RTC_alarm_duration(0, 0, 5, 0);
```

#### RTC Alarm At

Set RTC alarm for a specific time.

`bool RTC_alarm_at(DateTime future_time);`

`bool RTC_alarm_at(uint hour, uint minute, uint second);`

Example:

```cpp
// DateTime time
// Alarm at time specified by DateTime object `time`
InterruptManager().RTC_alarm_at(time);
// or
// Alarm at 5:30 pm
InterruptManager().RTC_alarm_at(17, 30, 0);
```

#### RTC Alarm Duration from Last

Set RTC alarm an amount of time from last alarm time.

`bool RTC_alarm_duration_from_last(TimeSpan duration);`

`bool RTC_alarm_duration_from_last(uint days, uint hours, uint minutes, uint seconds);`

Example:

```cpp
// Alarm 300 seconds from last alarm time with TimeSpan
InterruptManager().RTC_alarm_duration_from_last(TimeSpan(300));
// or
// Alarm 5 minutes after last alarm
InterruptManager().RTC_alarm_duration_from_last(0, 0, 5, 0);
```

### More

See the [Interrupt Manager documentation](http://web.engr.oregonstate.edu/~goertzel/Loom_documentation/class_loom___interrupt___manager.html) for complete list of available methods.

## Sleep Manager

The `Loom_Sleep_Manager` is used to put the board into a low power or off state. It will also power off any hardware with its own power off functionality. In order to wake from sleep or turn back on, you must have  an interrupt (or interrupts) setup to wake the device from sleep, see the [Interrupt Manager](#interrupt-manager) section for details on doing this.

#### Sleep

The board will go into low power mode and continue from the same location upon waking.  

```cpp
Manager.SleepManager.sleep();
// program will continue here upon waking
```

***Note:*** see [Example ISRs](#example-isrs) for the type of ISR to link to associate with an interrupt that wakes the device  

#### Power Off

The board will turn completely off, saving important information to flash or or SD, if you have information that you want to persist, make sure it is saved, because variables will be reset upon restart. The program will restart from `setup()` upon waking. This operation assumes that you are using an OPEnS Low Power board to facilitate turning the device completely off and restarting with external interrupts / RTC.

```cpp
Manager.SleepManager.powerDown();
// program will not run anything after powerDown if successful
// will instead restart at setup()
```

### More

See the [Sleep Manager documentation](http://web.engr.oregonstate.edu/~goertzel/Loom_documentation/class_loom___sleep___manager.html) for complete list of available methods.

## Multiplexer

The multiplexer is a hardware shield (stacks on Arduino Feather) that allows you to easily attach several I^2^C sensors to your Loom device, rather than having to wire them all individually. The multiplexer has a corresponding `LoomModule` that automates the management of any sensors currently plugged into it (can automatically detect new sensors and instantiate sensor modules to handle them). 

While you can call the  methods of the `Loom_Multiplexer` module directly, you can generally just let the `LoomManager` manage the multiplexer as well. In addition to typical `LoomModule` methods, the multiplexer allows operations such as the following:

#### Get Sensor List

This returns a Json object listing the sensors currently plugged in.

```cpp
// JsonObject sensorList
// Loom_Multiplexer mux
mux.get_sensor_list(sensorList);

// or 
// LoomManager Manager
Manager.Multiplexer.get_sensor_list(sensorList);
```

#### Refresh Sensors

This will force the multiplexer to update the sensor list, regardless of normal update interval, if any (note that automatic updating of sensors can be enabled by setting `dynamic_list` to `true`. The minimum time between updates is `update_period`)

```cpp
mux.refresh_sensors();
// or 
Manager.Multiplexer.refresh_sensors();
```

#### Get Sensor

You can get a pointer to a sensor plugged into a port of the multiplexer with:

```cpp
int index = 0; // 0 to (number of multiplexer ports)-1, normally 8
LoomI2CSensor* sensor =	mux.get_sensor(index);
// or 
LoomI2CSensor* sensor =	Manager.Multiplexer.get_sensor(index);
```

### More

See the [Multiplexer documentation](http://web.engr.oregonstate.edu/~goertzel/Loom_documentation/class_loom___multiplexer.html) for complete list of available methods.
