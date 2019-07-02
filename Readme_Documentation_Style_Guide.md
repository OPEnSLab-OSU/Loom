# Loom Documentation Style Guide

In order for [Doxygen](http://www.doxygen.nl) to be able to generate documentation of the code, comments need to be in a particular format. There a are variety of supported styles of comments, so for consistency Loom follows a subset of these styles. The preferred way to comment typical elements of Loom code are detailed below

## General Code

### Comment Blocks

#### Brief

```cpp
/// Brief description which ends at this dot. Details follow
/// here.
```

#### **Detailed**

```cpp
///
/// ... text ...
///
```

### **After variables, members, and enum items**

```cpp
int var; ///< Brief description after the member
```

```cpp
int var; ///< A more detailed description 
				 ///< after the variable
```

## Configurator Documentation

Loom has a [configuration web app](https://github.com/OPEnSLab-OSU/Loomify) that will output device configuration's based on user input to a graphical user interface. In order for any new classes to appear as options (automatically) in the configurator, the class must follow specific documentation described below.

### Class Description

Toward the top of each header file, immediately before the the class declaration, put text that matches the following format:

```cpp
// ### (<inheritFromModule>) | dependencies: [] | conflicts: []
/// <Description of module>
// ###
```

Angle brackets indicate a required field. Do not include them in your code.

- "inheritFromModule": The virtual class of which your module will inherit methods
- "Description of module": A short description of your module.

#### Example found in Loom_OLED class.

```cpp
// ### (LoomLogPlat) | dependencies: [] | conflicts: []
/// OLED logging platform module.
// ###
class Loom_OLED : public LoomLogPlat
{

protected:
... /*rest of class code... */
```

### Constructor

Immediately before the constructor for your class put text that matches the following format:

```cpp
/// <moduleName> module constructor.
///
/// \param[<in,out>] <paramName> <paramDataType> | <paramValue> | <paramRange> | <Description of param>
/// ...
/// ...
```

- **moduleName**: the name of the module, which is also the name of the class

- **in,out**: The parameter will either be in and/or out
- **paramDataType**: The C++ data type of the parameter
- **paramValue**: The actual value of the parameter, following C++ syntax for all data types values
- **paramRange**: If the range of acceptable values for the parameter are discreet, then this will be a comma separated list inside curly braces. If the acceptable range is continuous then this will be a two hyphen separated values inside brackets. If a range value does not make sense for the data type then this value will be null.
- **Description of param**: Write a brief description of this parameter

#### Example found in Loom_OLED class.

```cpp
/// OLED module constructor.
///
/// \param[in]	module_name	String | <"OLED"> | null | OLED module name
/// \param[in]	enable_rate_filter	Bool | <true> | {true, false} | Whether or not to impose maximum update rate
/// \param[in]	min_filter_delay	Int | <300> | [50-5000] | Minimum update delay, if enable_rate_filter enabled
/// ...
/// ...
Loom_OLED(
    const char*		module_name						= "OLED",
    bool					enable_rate_filter		= true,
    uint					min_filter_delay			= 300,
    ...
    ...
  );
```

#### Notes

Make use of tabs for alignment of elements of the documentation for readability. 



