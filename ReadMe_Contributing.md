# Contributing to Loom

## Table of Contents

- [Create Branch or Fork](#create-branch-or-fork)
- [Starting a Module Class](#starting-a-module-class)
- [Implementing a Module](#implementing-a-module)
- [Documenting Additions](#documenting-additions)
- [Creating an Example](#creating-an-example)
- [Making a Pull Request](#making-a-pull-request)

## Create Branch or Fork



## Starting a Module Class

In many cases, additions to Loom will take the form of adding a class inheriting from LoomModule in order to represent a new hardware component.

### Templates

If possible, use one of our provided templates to start your module to maintain consistency and make it clear what you need to implement. The class [hierarchy graph](http://web.engr.oregonstate.edu/~goertzel/Loom_documentation/inherits.html) in the documentation may be useful for determining where in the inheritance tree your module fits. Pick the best match to your module from the following templates (which can be found in the library under /Loom/Templates or on [GitHub](https://github.com/OPEnSLab-OSU/Loom/tree/master/Templates)):

- Module 
- Sensor
  - I2C sensor
  - SPI sensor
  - SDI-12
  - General / other sensor
- Actuator
- Logging platform
- Communication platform
- Internet platform
- Publishing platform

## Implementing a Module

The templates are intended to give you a clear direction when implementing your class. Often, the example code for a component translates well into the methods of a `LoomModule`. Please make use of [Loom's style guide](https://github.com/OPEnSLab-OSU/Loom/blob/master/Readme_Documentation_Style_Guide.md) for clean, consistent code and documentation. It is expected that you fully document your class. In addition to the templates and the style guide, looking at other classes in Loom can guide your implementation.

### Naming

Loom uses the following naming scheme with UpperCamelCasing:

#### Abstract Class

`Loom<module-name>` e.g: `LoomSensor` (no underscore)

An abstract class cannot be instantiated but is used to contain common functionality of the subclasses that inherit from it, and define an interface that those subclasses must implement. The class should be named such that any class inheriting from it "is a" `<module-name>`. For example with a Loom_**Relay** module, which inherits from LoomActuator, a **Relay** is an actuator.

#### Instantiatable Class

`Loom_<module-name>` e.g: `Loom_Relay` (with underscore)

These types of classes are what will be instantiated, used to interface with hardware, managed by `LoomManager`, etc. 

### Constructor

This is where you will setup the variables of your class instance and setup the device. If you have a setup process that may happen again later in operation, put it in another method and call it in the constructor. 

There are two constructors that should be implemented (though you can implement more if necessary).

One takes all of the necessary arguments to configure the class. The parameters can have default values. See the constructor section of the [Style Guide](https://github.com/OPEnSLab-OSU/Loom/blob/master/Readme_Documentation_Style_Guide.md#constructor) for proper documentation of this constructor.

The other constructor only takes a `JsonArrayConst` and expands the array, forwarding the elements of the array as arguments to the other constructor (this known as constructor delegation).

For example, the Loom_TSL2591 has a constructor of the form:

```cpp
// in .h	
Loom_TSL2591(
    byte          i2c_address    = 0x29,
    const char*   module_name    = "TSL2591",
    uint8_t       gain_level     = 1,
    uint8_t       timing_level   = 0
  );
```

The other constructor:

```cpp
// in .h
Loom_TSL2591(JsonArrayConst p);

// in .cpp
Loom_TSL2591::Loom_TSL2591(JsonArrayConst p)
	: Loom_TSL2591( EXPAND_ARRAY(p, 4) ) {}
```

Because the first constructor has **4** parameters, the second constructor delegates to the first with `EXPAND_ARRAY(p, 4)`. 

**Note:** If one of your parametes is an enum, you will likely need to cast the argument to an `int` instead. So rather than using `EXPAND_ARRAY`, use something like the following (example from Loom_DS3231 RTC module):

```cpp
Loom_DS3231::Loom_DS3231(JsonArrayConst p)
	: Loom_DS3231(p[0], (TimeZone)(int)p[1], p[2], p[3], p[4]) {}
```

### Destructor

If you allocate memory, you need to implement the destructor to free the memory. If you do not have any deinitialiation code to run, you can use: `~Loom_<module-name>() = default` to indicate that the compiler should generate the default destructor.

**Note:** label as `virtual`

### Print Methods

Before printing information, it is encouraged that you call `print_module_label()`, which will print the name of the module so that it is clear in the Serial monitor which module is printing the information.

**Print Verbosity:** All modules inherit a `print_verbosity` member from `LoomModule`, and has options of `Verbosity::V_OFF`, `Verbosity::V_LOW`, `Verbosity::V_HIGH` for off, low, and high verbosities, respectively. Print statements should be contained if conditionals such that if the verbosity is set to off nothing prints. You do not have to distinquish low and high verbosity, but you can set up to statements that some less commonly needed print statements are only printed if verbosity is high.

#### Print Config

Use this method to print information about the configuration / settings of the module. 

For example, all `LoomModule`s print use at least the following:

```cpp
void LoomModule::print_config()
{
	print_module_label();
	LPrintln("Config:");
	LPrintln('\t', "Module Active       : ", (active)      ? "Enabled" : "Disabled" );
	LPrintln('\t', "Print Debug         : ", (print_debug) ? "Enabled" : "Disabled" );
	LPrintln('\t', "Print Verbosity     : ", enum_verbosity_string(print_verbosity) );
	LPrintln('\t', "Package Verbosity   : ", enum_verbosity_string(package_verbosity) );
}
```

And subclass should call the superclass implementation of `print_config()` before adding any additional print statements. This allows your module to print all config information relevant to it, even if the setting pertains to a superclass. See the following `Loom_LoRa` module (which inherits from `LoomCommPlat`) example:

```cpp
void Loom_LoRa::print_config() 
{
	LoomCommPlat::print_config();

	LPrintln('\t', "Address             : ", address );
	LPrintln('\t', "Power Level         : ", power_level );
	LPrintln('\t', "Retry Count         : ", retry_count );
	LPrintln('\t', "Retry Timeout       : ", retry_timeout );
}
```

 If your module adds no new configuration to what the superclass has, you can omit implementing the `print_config()` method (leave out of .h and .cpp), in which case the superclass's version will be run. 

**Note:** label as `override`

#### Print State

Print state is similar to print config, but state pertains less to settings and more to the current state of the module / hardware it is managing. For example, given a servo module, the angle of the servo is part of the state, not the config. Sort information into state and config as makes most sense. Here is the `Loom_Servo`s implementation as an example:

```cpp
void Loom_Servo::print_state()
{
	print_module_label();
	LPrintln('\t', "Servo Positions:" );
	for (int i = 0; i < servo_count; i++) {
		LPrintln('\t\t', "Degree ", i, ": ", positions[i]);
	}
}
```

**Note:** label as `override`

#### Print Measurements (sensor only)

Use this method to print data measurements collected. As with `print_config` and `print_state()`, you can make use of the `print_verbosity` setting, perhaps printing converted values at low verbosity, but also printing raw values at high verbosity. The following example is the TSL2591 lux sensor module implementation.

```cpp
void Loom_TSL2591::print_measurements()
{
	print_module_label();
	LPrintln("Measurements:");
	LPrintln("\t", "Vis  : ", vis);
	LPrintln("\t", "IR   : ", ir);
	LPrintln("\t", "Full : ", full);
}
```

**Note:** Label as `override`

### Measure (sensors only)

This method is was will be called to instruct a sensor to collect data and copy it into a member variable(s). This does not package data into Json, for that, see [Package](#package) below. Often times, the `LoomSensor` modules contain another library's class for interfacing with the sensor itself. In that case, `measure()` generally entails making calls that class's method for measuring data. The following is the `Loom_TSL2591` module's implementation:

```cpp
void Loom_TSL2591::measure()
{
  vis  = inst_tsl2591.getLuminosity(TSL2591_VISIBLE);
  ir   = inst_tsl2591.getLuminosity(TSL2591_INFRARED);
  full = inst_tsl2591.getLuminosity(TSL2591_FULLSPECTRUM);
}
```

If your sensor has different modes, you can use a member of the class to track the current mode and switch what data to collect in the `measure()` method.

**Note:** Label as `override`

### Package

The `package()` method is called to copy the measured data (stored in member variables) into a standardized format within a `JsonObject`. This method is typically called by a `LoomManager` by iterating over modules sending them a Json object to add to, but a user can call the method directly if they need to.

This method is easy to implement, using the provided `template<typename... Args> void package_json(JsonObject json, const char* module_name, const Args... args)` (see the [documentation](http://web.engr.oregonstate.edu/~goertzel/Loom_documentation/_loom___package_8h.html) for full details on this function). The function works by sending the `JsonObject` sent to `package()`, the module name, and then alternating keys and values. The keys need to be c-strings, the values can be anything. Here is the `Loom_TSL2591` example:

```cpp
void Loom_TSL2591::package(JsonObject json)
{
	package_json(json, module_name, 
		"Vis",  vis,
		"IR",   ir,
		"Full", full
	);
}
```

**Package Verbosity:** Similar to print statements, `package()` can make use of the `package_verbosity` setting (same options of:`Verbosity::V_OFF`, `Verbosity::V_LOW`, `Verbosity::V_HIGH` for off, low, and high verbosities, respectively). You can use if statements or a switch statement to call `package_json(…)` with different arguments depending on the current verbosity level.

**Note:** label as `override`

### Calibrate (sensors only)

*In progress*

### Support for Executing Commands

The `bool cmd_route(JsonObject)` method pertains mostly to running commands to control an actuator, but can  be implemented by any module to execute an of its methods based on a command JsonObject. Using the `template<typename FName, typename FType, typename... Args> bool functionRoute(const char* name, const FName fName, const FType f, const Args... args)` function you can associate strings with methods of the class to execute, with the arguments provided in the JsonObject command. See the [documentation](http://web.engr.oregonstate.edu/~goertzel/Loom_documentation/_loom___misc_8h.html) if you want more details on that function. 

The function takes a c-string representing a function to call, and then alternating c-strings and lambda functions that expand the json command's arguments to arguments to a method of the class.

The following is an example of the `Loom_Servo` implementation:

```cpp
bool Loom_Servo::cmd_route(JsonObject json)
{
	if ( strcmp(json["module"], module_name) == 0 ) {
		JsonArray params = json["params"];
		return functionRoute(
			json["func"],
			"set_degree", [this, params]() { if (params.size() >= 2) { set_degree( EXPAND_ARRAY(params, 2) ); } else { LPrintln("Not enough parameters"); } } // modify this line
		);
	} else {
		return false;
	}
}
```

You don't have to worry much about the how this works, just what you need to change to get it to work with your module. The only part that you need to modify from the above is the arguments to `functionRoute`. Leave `json["func"]` as the first argument. The rest of the arguments are the alternating strings and lambda. 

- The string should be or represent the name of the method you want to execute. In the above example this is the string `"set_degree"`
- The lambda function will stay mostly the same, but you will change:
  - `if (params.size() >= 2)` – change the **2** to the number to match the number of parameters the method you want to execute has
  - `set_degree` – change to the name of the method you want to be executed
  - `EXPAND_ARRAY(params, 2)` – change the **2** to the number to match the number of parameters the method you want to execute has

If you have multiple commands you want to support, just add more pairs of strings and lambdas.

If your module does not need to implement executing commands from Json, you can omit the function from the .h and .cpp. 

**Note:** label as `override`

### Power Up and Power Down

*In progress*

### Auxiliary Methods

If you have any functions that are not commands that a user should have access to, make them private as to make your module easier to understand and use.

### Enums and Structs

If you define any new enumerations and they pertain only to your new module, put them in the definition of the module class.

The following is a part of the `Loom_OLED` module definition, demonstrating how to define and use an enum in a class:

```cpp
class Loom_OLED : public LoomLogPlat
{
public:
	/// Different forms of the OLED display
	enum class Version {
		FEATHERWING,  ///< FeatherWing OLED
		BREAKOUT      ///< Breakout board
	};

	/// Different formats to display information in
	enum class Format {
		FOUR,       ///< 4 Key values
		EIGHT,      ///< 8 Key values
		SCROLL      ///< Scrolling
	};
	...
}
```

Within the class's methods, you can use the enum as `<enum-name>::<enum-value>`. Following the `Loom_OLED` class example, the following are some of the enum values: 

```cpp
Version::FEATHERWING
Version::BREAKOUT
Format::SCROLL
```

If you are referring to the enum outside of the class (as long as the enum is public), you need to specify the value as `<module-name>::<enum-name>::<enum-value>` in order to indicate which class the enum is associated with. Example:

```cpp
Manager.OLED().set_display_format(Loom_OLED::Format::SCROLL);
```

The same approach applies to structs associated with only that module. An example is the structs the `Loom_Interrupt_Manager` uses (note that documentation of it is truncated for this example):

```cpp
class Loom_Interrupt_Manager : public LoomModule
{

private:

	/// Contains information defining an interrupt's configuration
	struct IntDetails {
		ISRFuncPtr   ISR;         ///< Function pointer to ISR. Set null if no interrupt linked
		byte         type;        ///< Interrupt signal type to detect. LOW: 0, HIGH: 1, ...
		ISR_Type     run_type;    ///< True if ISR is called directly ...
		bool         enabled;     ///< Whether or not this interrupt is enabled
	};
  ...
};
```

Make the structs `private` if the user / other classes do not need to use the classes.

### Other Notes

#### Virtual Methods

Some of the classes your module inherits from may define pure virtual methods (denoted by a leading `virtual` keyword). These are used with polymorphism to make sure the right version of the method is called. If the superclass's implementation of the method is appropriate for you module you do not have to implement that method. Otherwise you can override is by defining a method of the same name. Label these methods with the leading keyword `virtual` and the trailing keyword `override` in the .h file. When overriding a method, you can access the superclass's version with `superclass-name::method-name(…arguments…)` as is done in many of the `print_config()` implementations

#### Pure Virtual Methods

Pure virtual methods are similar to virtual methods (denoted by a leading `virtual` keyword and a trailing `= 0`), but instead of giving you the **option** to override the superclass's implementation, the superclass does not provide any implementation and you **must** provide implementation of the method in you derived class. Label these methods with the leading keyword `virtual` and the trailing keyword `override` in the .h file. 

## Code that Needs to be Updated

While Loom aims to keep the number of locations in code that need to be editted with the addition of a new module to a minumum, there are a few locations outside of your new module files that will need to be updated. These are mostly adding elements for your module to enums and arrays.

Described below are the locations in the code, and what you should add.

### Loom.h

#### Include

If your class is non-abstract add an include for your module's .h file in "Loom.h" in the form:

```cpp
#include "path/to/header/file.h"
```

Example:

```cpp
#include "Sensors/I2C/Loom_TSL2591.h"
```

The includes are sorted into similar types, determine where you class fits best and add the include at the end of that list.

### LoomModule.h

- Type enum

### Manager.h

#### Forward Declaration

In order for the `LoomManger` class to be aware of your new class, add a forward declaration of your class near the top of the file in the form:

```cpp
class Loom<module-name>; // abstract
class Loom_<module-name; // non-abstract
```

**Example:**

```cpp
class LoomSensor;
class Loom_TSL2591;
```

The forward declarations are sorted into similar types, determine where you class fits best and add the declaration at the end of that list.

#### Module Access

In order for the Manager to allow access the the methods of the modules it manages, it defines a method corresponding to the name of the class it accesses. If your class is **non-abstract**, in the MODULE ACCESS section (ctrl+F to find it) add a method of the form (note that the "Loom" prefix of the class name is omitted from the method name):

```cpp
<Loom-class_name>& module-type(int idx = 0);
```

**Example:**

```cpp
Loom_TSL2591& TSL2591(int idx = 0);
```

The methods are sorted into similar types, determine where you class fits best and add the method at the end of that list.

### Loom_Manager_Module_Access.cpp

This file is dedicated to the implementation of the module access methods described [above](#module-access), and only for **non-abstract** modules. These methods all look very similar, form:

```cpp
<Loom-class-name>& LoomManager::<class-name>(int idx)
{
  	LoomModule* tmp = find_module(LoomModule::Type::<module-type>, idx, <manager-module-list>);
	return (*(<Loom-class-name>*)( (tmp) ? tmp : &global_stub ));
}
```

**Note:** The `<module-type>` refers to the `LoomModule::Type` enum value. See the above section on [adding to LoomModule.h](#loommodule.h) for details on this enum.

**Note:** The `manager-module-list>` refers to which vector the Manager keeps pointers to your module in. Sensor modules go in `sensor_modules`, actuators in `actuator_modules`, etc. If you module does not fit any of the existing lists, you will probably add it to `other_modules`. See the "Protected Attributes" section of the `LoomManager` [documentation](http://web.engr.oregonstate.edu/~goertzel/Loom_documentation/class_loom_manager.html) or the protected members of the `LoomManager` class declaration in "Loom_Manager.h" file for the available vectors to sort into.

**Example:**

```cpp
Loom_TSL2591& LoomManager::TSL2591(int idx) 
{
	LoomModule* tmp = find_module(LoomModule::Type::TSL2591, idx, sensor_modules);
	return (*(Loom_TSL2591*)( (tmp) ? tmp : &global_stub ));
}
```

The methods are sorted into similar types, determine where you class fits best and add the method at the end of that list.

### Loom_Module_Stub.h

The `Loom_Module_Stub` is used to prevent runtime errors if you try to use the module access functions of the `LoomManager` for a module that does not exist. If your module is not found, rather than returning a `null_ptr` it returns the stub, which will print out an error message for the method you attempted to call.

If your class is **non-abstract**, for any public methods you implemented that do not implement/override any superclasses' methods, add a version of the method to the module stub. The method should have the same signature as the one in your class, but the body is defined in the header and just prints an error.

**Example:**

```cpp
void set_degree(int servo, int degree)
{ LPrintln(error_msg); }
```

### Loom_Module_Factory.h

The `LoomManager` uses the `Factory` class to instantiate any of the non-abstract `LoomModule`s as defined by a Json configuration.

At the top of the "Loom_Module_Factory.h" file, there are a number of `#includes`. If your class is non-abstract add an include for your module in the form:

```cpp
#include "path/to/header/file.h"
```

**Example:**

```cpp
#include "Sensors/I2C/Loom_TSL2591.h"
```

The includes are sorted into similar types, determine where you class fits best and add the include at the end of that list.

### Loom_Module_Factory.cpp

The `Factory` also makes use of a lookup table to associate strings representing a module's name (this string is what is provided in a Json configuration to specify which module to instantiate), the constructor for the module, and a sort type enum value, all combined into a struct. The elements of the array are in the form:

```cpp
{<class-name>,			Construct<class-name>,		ModuleSortType::<sort-type> },
```

**Example:**

```cpp
{"Loom_TSL2591",			Construct<Loom_TSL2591>,		ModuleSortType::Sensor },
```

Available `ModuleSortType` options can be found in the "Loom_Module_Factory.h" file 

The array is sorted into similar types, determine where you class fits best and add the include at the end of that section.

### Multiplexer.h

Some I2C sensors have conflicting addresses. At the top of this file is an enum of sensors that might have a conflict, followed by a series of constants specifying what sensor to interpret those I2C addresses as. 

If you sensor's address can conflict with another add to the enum in the form:

```cpp
L_<class-name>	///<<class-name>
```

**Example:**

```
L_TSL2591,		///< TSL2591
```

For any potentially conflicting address, make sure there is a define for that address in the form:

```cpp
const I2C_Selection i2c_<address> = I2C_Selection::L_<class-name>; // possible / sensors / that / use / that / address
```

**Example:**

```cpp
const I2C_Selection i2c_0x29 = I2C_Selection::L_TSL2591;	// TSL2561 / TSL2591
```

If the address in question already has a sensor it is set to, do not override it unless a Loom admin (or for testing. If your sensor conflicts with an existing Loom supported sensor, the define should like select the already implemented sensor.

### Multiplexer.cpp (for I2C sensors)

If your module is for an I2C sensor, you will also need to update a few locations in the multiplexer code.

#### Include

Near the top of the file is a list of `#includes`, add an include for your sensor in the form:

```cpp
#include "Sensors/I2C/<class-name>.h"
```

**Example:**

```cpp
#include "Sensors/I2C/Loom_TSL2591.h"
```

#### Known Addresses

When polling the ports of the multiplexer, the `Loom_Multiplexer` is optimized to only check for addresses of I2C sensors Loom supports. 

For **each** possible I2C address of you sensor, add an element to the `known_addresses` array in the form:

```cpp
<address> // Sensor
```

**Example:**

```
0x44, // SHT31D
0x45, // SHT31D
```

#### Generate Sensor Object

Once the multiplexer knows the I2C address of the sensor plugged into a port, it needs to generate a `LoomI2CSensor` module to manage the sensor. The `Loom_Multiplexer` uses a switch statement to select which sensor to generate. 

If your sensor does not have a potential address conflict add a case in the form (note that you might be adding multiple cases if you sensor supports multiple addresses):

```cpp
case <address> : return new <class-name>(i2c_address=<address>);	// sensor name
```

**Example:**

```cpp
case 0x39 : return new Loom_TSL2561(i2c_address=0x39);	// TSL2561
```

If your sensor does have an address conflict, for the conflicting address, add an if statement for you sensor (and potentially the other sensor it conflicts with) checking the enum value specified in the "Loom_Multiplexer.h" file (see [above](#multiplexer.h)) in the same fashion as the following example:

```cpp
case 0x49 : // TSL2561 / AS726X / AS7265X
			if (i2c_0x49 == I2C_Selection::L_TSL2561) return new Loom_TSL2561(i2c_address=0x49);	// TSL2561
			if (i2c_0x49 == I2C_Selection::L_AS7262 ) return new Loom_AS7262(i2c_address=0x49);	// AS7262
			if (i2c_0x49 == I2C_Selection::L_AS7263 ) return new Loom_AS7263(i2c_address=0x49);	// AS7263
			if (i2c_0x49 == I2C_Selection::L_AS7265X) return new Loom_AS7265X(i2c_address=0x49);	// AS7265X		
```

### Other

- Other additions may be necessary if your module interacts with other modules
- If a superclass declares a non-pure method that your class does not need to implement, simply remove the method when using the templates rather than leaving an empty body as an implementation
- You do not necessarily need to add new documentation for a method that your class overrides if the superclass's documentation suffices.

## Documenting Additions

Follow our [style guide](https://github.com/OPEnSLab-OSU/Loom/blob/master/Readme_Documentation_Style_Guide.md) for documentation and ensuring that any new module classes appear as options in the configurator and that the methods are documented in the Doxygen generated documentation.

If you are part of the OPEnS lab, you might also add information to the [wiki](#https://wiki.open-sensing.org/doku.php?id=loom).

## Creating an Example

We also encourage you to make a basic example(s) of using your module using Loom. This can resemble the example code of the hardware if it has an existing library that provides an example. The example(s) can demonstrate usage with or without the `LoomManager`. 

To create an example you will need generally to write a .ino file to use the module and a config.h file to specify the configuration (if using the `LoomManager`). You can use other examples as to guide your example. This typically amounts to showing the usage of the operation methods to demonstrate its intended functionality.

Once you have written and tested you example(s), you can copy it into the Loom/examples directory. Note that there are number of folders in the directory, put your example in the category it matches best. If you restart Arduino, your example should be available from the File > examples > Loom dropdown menu.

## Making a Pull Request

