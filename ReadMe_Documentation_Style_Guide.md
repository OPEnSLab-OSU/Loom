# Documentation Style Guide

## Comment blocks

```
/// ... text ... 
/// ... text ...
/// ... text ...
```

```
/// Brief description which ends at this dot. 
/// Details follow here.
```

## Variable Description

```	
/// Description of variable
int var;
```

## Inline Comments

Prefer for a sequence of similarly formatted lines of code (e.g. switch statements, enum definitions)

```
code		///< Brief inline comment
```

```
code		///< Detailed inline
			///< comment
```

## Function/Method Comment Blocks

**Function declaration (in .h file)**

```
/// Brief description.
/// Detailed description
/// goes here.
/// \param[in]		param1		Brief description of param1		
/// \param[out]		param2		Brief description of param2
/// \param[in,out]	param3		Brief description of param3
/// \return			Brief description of return value
bool func(float param1, int& param2, char* param3);

```

*Note:* Function templates are declared and defined in header file, but the above comment block will apply

**Function Definition (in .cpp file)**

Only has a divider between other function definitions, description goes in header file

```
/////////////////////////////////////////////////////////////////////
int func(float param1, int& param2, char* param3)
{
	... 
    code implementation
    ...
}

```



## Code Divider / Heading

**Major**

```
// ================================================================
// ===                Major Section Description                 ===
// ================================================================
```

**Minor**

```
// === === Minor Section Description === ===
```

## Non-Documentation Comments

General Comments

```
// Comment
code being 
commented on
```

Series of short statements can also be commented as

```
statement1;			// comment 1
statement2;			// comment 2
statement3;			// comment 3

```



