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

If possible, use one of our provided templates to start your module to maintain consistency and make it clear what you need to implement. The class [hierarchy graph](http://web.engr.oregonstate.edu/~goertzel/Loom_documentation/inherits.html) in the documentation may be useful for determining where in the inheritance tree your module fits. Pick the best match to your module from the following templates:

- Module 
- Sensor
  - I^2^C sensor
  - SPI sensor
  - SDI-12
  - General / other sensor
- Actuator
- Logging platform
- Communication platform
- RTC
- Internet platform
- Publishing platform

## Implementing a Module

The templates are intended to give you a clear direction when implementing your class. Often, the example code for a component translates well into the methods of a `LoomModule`. Please make use of [Loom's style guide](https://github.com/OPEnSLab-OSU/Loom/blob/master/Readme_Documentation_Style_Guide.md) for clean, consistent code and documentation. It is expected that you fully document your class. In addition to the templates and the style guide, looking at other classes in Loom can guide your implementation.

### Naming

Loom uses the following naming scheme with UpperCamelCase:

#### Abstract Class

`Loom<module-name>` e.g: `LoomSensor` (no underscore)

An abstract class cannot be instantiated but is used to contain common functionality of the subclasses that inherit from it, and define an interface that those subclasses must implement. The class should be named such that any class inheriting from it "is a" `<module-name>`. For example with a Loom_**Relay** module, which inherits from LoomActuator, a **Relay** is an actuator.

#### Instantiatable Class

`Loom_<module-name>` e.g: `Loom_Relay` (with underscore)

These types of classes are what will be instantiated, used to interface with hardware, managed by `LoomManager`, etc. 

### Constructor

This is where you will setup the variables of your class instance and setup the device. If you have a setup process that may happen again later in operation, put it in another method and call it in the constructor. 

There are two constructors that should be implemented (though you can implement more if necessary).

One takes all of the necessary arguments to configure the class. The parameters can have default values. See the constructor section of the [Style Guide](https://github.com/OPEnSLab-OSU/Loom/blob/cleanup_and_doxygen/Readme_Documentation_Style_Guide.md#constructor) for proper documentation of this constructor.

The other constructor only takes a `JsonArrayConst` and expands the array, forwarding the elements of the array as arguments to the other constructor (this known as constructor delegation).

For example, the Loom_TSL2591 has a constructor of the form:

```cpp
// in .h	
Loom_TSL2591(
    byte					i2c_address			= 0x29,
    const char*		module_name			= "TSL2591",
    uint8_t				gain_level			= 1,
    uint8_t				timing_level		= 0
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

### Destructor

If you allocate memory, you need to implement the destructor to free the memory. If you do not have any deinitialiation code to run, you can use: `~Loom_<module-name>() = default` to indicate that the compiler should generate the default destructor.

Label as `virtual`

### Print Methods

Before printing information, it is encouraged that you call `print_module_label()` which will print the name of the module so that it is clear in the Serial monitor which module is printing the information.

**Print Verbosity:** All modules inherit a `print_verbosity` member from `LoomModule`, and has options of `Verbosity::V_OFF`, `Verbosity::V_LOW`, `Verbosity::V_HIGH` for off, low, and high verbosities, respectively. Print statements should be contained if conditionals such that if the verbosity is set to off nothing prints. You do not have to distinquish low and high verbosity, but you can set up to statements that some less commonly needed print statements are only printed if verbosity is high.

#### Print Config



Label as `override`

#### Print State



Label as `override`

#### Print Measurements (sensor only)

Use this method to print our data measurements collected

**Example:**

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

Label as `override`

### Measure (sensors only)

### Package

Label as `override`

Package Verbosity

### Calibrate (sensors only)

### Support for Executing Commands



### Power Up and Power Down

### Auxiliary Methods

If you have any functions that are not commands that a user should have access to, make them private.

### Enums and Structs

### Other Notes

- Implementing (pure) virtual functions
- When to use override
- When to use label as (pure) virtual 

## Code that Needs to be Updated

While Loom aims to keep the number of locations in code that need to be editted which the addition of a new module to a minumum, there are a few locations outside of your new module files that will need to be updated. These are mostly adding elements for your module to enums and arrays.

Described below are the locations in the code, and what you should add to.



- Type enum 
- Factory
- Loom.h?



## Documenting Additions

Follow our [style guide](https://github.com/OPEnSLab-OSU/Loom/blob/master/Readme_Documentation_Style_Guide.md) for documentation and ensuring that any new module classes appear as options in the configurator.

## Creating an Example



## Making a Pull Request