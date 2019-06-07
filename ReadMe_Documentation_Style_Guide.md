# Documentation Style Guide

## Variable Description

A comment describing a variable should look like:

```cpp
/// Description of variable
int var;
```

Alternatively, if there are several similar variables in sequence, they may be commented as such:

```cpp
int var1;			///< Description of var1
int var2;			///< Description of var2
int var3;			///< Description of var3
```

## Class Description

A comment describing a variable should look like:

```cpp
/// Description of class
class ExampleClass
{
  ...
}
```

## Inline Comments

Prefer for a sequence of similarly formatted, short lines of code (e.g. switch statements, items of enum definitions)

```cpp
code		///< Brief inline comment
```

```cpp
code		///< Detailed inline
				///< comment
```

## Function/Method Comment Blocks

**Function declaration (in .h file)**

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

**Function Definition (in .cpp file)**

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

## Comment blocks

General comment blocks may look like the following:

```cpp
/// Brief description which ends at this dot. 
/// Details follow here.
```
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

## Code Divider / Heading

If sections of code within a file warrant dividers for readability and ease of navigation, headers / dividers should looks as follows. If code is different enough, it can likely go in its own files, rather than dividing it like this.

**Major**

```cpp
// ================================================================
// ===                Major Section Description                 ===
// ================================================================
```

**Minor**

```cpp
// === === Minor Section Description === ===
```

## Non-Documentation Comments

The comments in the code that are not used to generate documentation should generally follow a style as follows:

**General Comments**

```cpp
// Comment goes here
code being 
commented on
```

**Series of short statements**

A series of short lines of code may also be commented as:

```cpp
statement1;			// comment 1
statement2;			// comment 2
statement3;			// comment 3
```



