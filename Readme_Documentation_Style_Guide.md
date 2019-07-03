

# Loom Documentation Style Guide

In order for [Doxygen](http://www.doxygen.nl) to be able to generate documentation of the code, comments need to be in a particular format. There a are variety of supported styles of comments, so for consistency Loom follows a subset of these styles. The preferred way to comment typical elements of Loom code are detailed below

## General Code

### Variable, Member, or Enum Description

A comment describing a variable should look like:

```cpp
/// Description of variable
int var;
```

or 

```cpp
/// Description of variable.
/// Further details
int var;
```

Alternatively, if there are several similar variables in sequence, they may be commented as such:
```cpp
int var1;			/// Description of var1
int var2;			/// Description of var2
int var3;			/// Description of var3
```
Or
```cpp
int var1;			///< Description of var1
int var2;			///< Description of var2
int var3;			///< Description of var3
```
If a more detailed description is warranted, you may also use the following
```cpp
int var;			///< A more detailed description
							///< after the variable
```

### Class Description

A comment describing a class should look like the following unless it represents a module, in which case see the [details below](#configurator-documentation)

```cpp
/// Description of class
class ExampleClass
{
  ...
}
```

### Inline Comments

Prefer for a sequence of similarly formatted, short lines of code (e.g. switch statements, items of enum definitions)

```cpp
code		///< Brief inline comment
```

```cpp
code		///< Detailed inline
				///< comment
```

### Function/Method Comment Blocks

#### **Function declaration (in .h file)**

```cpp
/// Brief description.
/// Detailed description
/// goes here.
/// \param[in]			param1		Brief description of param1		
/// \param[out]			param2		Brief description of param2
/// \param[in,out]	param3		Brief description of param3
/// \return		Brief description of return value
bool func(float param1, int& param2, char* param3);

```

Leave a blank line after function declaration.

*Note:* Function templates are declared and defined in header file, but the above comment block will apply

*Note:* If documenting a constructor see the alternate form [detailed below](#configurator-documentation)

#### **Function Definition (in .cpp file)**

Only has a divider between other function definitions, description goes in header file

```cpp
/////////////////////////////////////////////////////////////////////
int func(float param1, int& param2, char* param3)
{
	... 
	code implementation
	...
}

```

Leave a blank line after function definition.

### Comment blocks

General comment blocks may look like the following:

#### Brief

```cpp
/// Brief description which ends at this dot. 
/// Details follow here.
```
#### Detailed
```cpp
/// ... text ... 
/// ... text ...
/// ... text ...
```

### Preprocessor Defines

A comment describing a preprocessor definition should look like:

```cpp
/// Description of define
#define DEFINITION	1
```

Alternatively, if there are several similar definitions in sequence, they may be commented as such:

```cpp
#define DEF1	123			///< Description of DEF1
#define DEF2	4.56		///< Description of DEF2
#define DEF3	"acb"		///< Description of DEF3
```

### Code Divider / Heading

If sections of code within a file warrant dividers for readability and ease of navigation, headers / dividers should looks as follows. If code is different enough, it can likely go in its own files, rather than dividing it like this.

#### **Major**

```cpp
// ================================================================
// ===                Major Section Description                 ===
// ================================================================
```

#### **Minor**

```cpp
// === === Minor Section Description === ===
```

### Non-Documentation Comments

The comments in the code that are not used to generate documentation should generally follow a style as follows:

#### **General Comments**

```cpp
// Comment goes here
... code being ...
... commented on ... 
```

#### **Series of short statements**

A series of short lines of code may also be commented as:

```cpp
statement1;			// comment 1
statement2;			// comment 2
statement3;			// comment 3
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



