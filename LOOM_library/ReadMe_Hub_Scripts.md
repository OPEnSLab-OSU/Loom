# Project Loom: Hub Scripts 

The Loom 'hub scripts' are essentially mini programs that run and a Loom device, especially hub-type devices. 

## Table of Contents



## Motivation 

Loom supports data processing and data flow control via the graphical interface and patches provided for Max/MSP. These provide powerful control of the devices and data of a Loom network, but require that the network is WiFi based (at least in part, repeaters to other communication platforms could partially get around this) and that a computer actively connnected to that network and running Max. This is impractical for most people for more permanent network deployments. 

The intent of these 'hub scripts' (as they will generally be referred to in the document) is to implement similar, albeit less powerful / flexible, processing and data control on one or more of the microprocessors on the network.

## Overview

The hub scripts are basic programs that support: 

- Computation based on data that can be:
  - Integer, float, boolean literals
  - One of the above types stored in particular variables (which are modelled as register banks)
- Reading from and writing to the above 'registers'
- Arbitrary function calls (generally through special wrapper functions)
- If statements (*currently without nesting*)
- Dynamical addition and removal of scripts (i.e. without reflashing the firmware)

## Implementation

The hub scripts are stored in OSC messages, though converted to and from String arrays before and after saving and reading from flash, respectively. This makes them easy to build, send, and parse. Further, it allows for native mixed data types (rather than converting all numbers and commands from a string to a number as needed, for example).

The programs are interpretted by a stack-based parser using Reverse Polish Notation (i.e. postfix notation). The basics about stack-based programming can be found [here](https://en.wikipedia.org/wiki/Stack-oriented_programming).

#### Notation

The notation means that operators follow the inputs to the operation. For example, `3 4 + ` in postfix notation is equivalent to normal (infix) `3 + 4`, where both evaluate to `7`. This eliminates the need for parentheses and allows math operations or other funtions to be applied to a stack of data.

#### Stack

While a program will be a mix of data literals, references to variables, if statements, and function calls, the stack is only for data. This stack only exists within the parser for the duration of the evaluation of a script, as it is added to, modified, and read from by the current script. Once the execution of a script has completed, you will not be able to access the contents anymore. If calculations placed on the stack are needed elsewhere, save the result to a register(s). Then call your own custom function to copy the data from these registers to other variable. Unlike the stack, however, the registers are globally accessible even after the completion of a script. The reason for copying to other variables would be to ensure that another script does not overwrite them (which could be avoided with careful design of scripts), and so that normal device operation can find the values in variables it uses for non-script operation (i.e. so that a device does not have mulitple locations where the value it needs could exist).

The stack itself is actually just an array of floats with a pointer to the 'top' (i.e. the index one greater than the current top of the stack). The motivation for selecting floats and not something more complex is becuase the data type is sufficiently flexible to support the 3 data types: int, float, and boolean. If a value needs to be interpretted by a function an integer, it can be cast as such. The same goes for booleans. A value of 0.0 represents false, while anything else is true.

## Creating Scripts

The script parser is relatively basic, and as such, only has minimal error checking. Well-formed scripts should not cause any errors, but incorrectly formatted scripts have undefined behavior (e.g. incorrect operation to freezing to reading/writing to memory where it shouldn't). Below are the supported data and function operations and the assumptions about the stack (or other) they make.

Commands of the script are executed from 0th index, increasing until the end of the message, only skipping commands as needed when contained within branches of if /else statements.

### Data Literals

A data (int, float, or boolean) literal is simply pushed to the top of the stack.

### Variable References

**Loading** (`"load_<bank><num>"`)

This reads a value from the referenced location / register and pushes the value to the top of the stack. Loading from one of the special register variables takes the form: `load_<bank><num>`, where `<bank>` is `r`, `s`, or `t` for different register banks, and `num` is 0-9. For example, `load_r7` reads the value from 8th register of the register bank 'R' and puts it on the stack.

**Storing** (`"store_<bank><num>"`)

Writes the value on the top of the stack to the specified register (see 'Loading' section above for notation on specifying which register). The value is **not** pushed from the stack. If that behavior is desired, the next command of the script should be `pop`. 

### Math

**Addition** (`"add"`) or (`"+"`)

Pops the top two values of the stack, adds them, and pushes the result.

**Subtraction** (`"subtract"`) or (`"-"`)

Pops the top two values of the stack, subtracts them, and pushes the result.

**Multiplication** (`"mulitply"`) or (`"*"`)

Pops the top two values of the stack, multiplies them, and pushes the result.

**Division** (`"divide"`) or (`"/"`)

Pops the top two values of the stack, divides them, and pushes the result.

**Exponentiation** (`"exp"`) or (`"^"`)

Pops the top two values of the stack, exponentiates them (where the top value is the power, and the value below it is the base), and pushes the result.

**Increment** (`"inc"`) or (`"++"`)

Increments the value on the top of the stack

**Decrement** (`"dec"`) or (`"--"`)

Decrements the value on the top of the stack

**Square** (`"square"`) or (`"sqr"`)

Squares the value on the top of the stack

### Number Comparisons

*Note: In the below comparisons, 'first' value is assumed to be the value 1 below the top of the stack, while the 'second' is the value on the top of the stack.* 

*Note: While these are generally used to be operators to numbers, they will still work on booleans because they are stored as floats.* 

**Equality** (`"equal"`) or (`"=="`)

Pops the top two values of the stack, pushes true (`1.0`) if they are the same, else false (`0.0`).

**Not Equal** (`"notEqual"`) or (`"!="`)

Pops the top two values of the stack, pushes true (`1.0`) if they are the different, else false (`0.0`).

**Less Than** (`"lessThan"`) or (`"<"`)

Pops the top two values of the stack, pushes true (`1.0`) if the first is less than the second, else false (`0.0`).

**Greater Than** (`"greaterThan"`) or (`">"`)

Pops the top two values of the stack, pushes true (`1.0`) if the first is greater than the second, else false (`0.0`).

**Less Than or Equal To** (`"lessThanEq"`) or (`"<="`)

Pops the top two values of the stack, pushes true (`1.0`) if the first is less than or equal to the second, else false (`0.0`).

**Greater Than or Equal To** (`"greaterThanEq"`) or (`">="`)

Pops the top two values of the stack, pushes true (`1.0`) if the first is greater than or equal to the second, else false (`0.0`).

### Logical Operations

*Note: In the below comparisons, 'first' value is assumed to be the value 1 below the top of the stack, while the 'second' is the value on the top of the stack.* 

*Note: While these are generally used to be operators to booleans, they will still work on numbers, where `0.0` is `false`, and anything else is `true.`* 

**Not** (`"not"`) or (`"!"`)

Pops the top value from the stack, pushes true (`1.0`) if was false (`0.0`), else pushes false (`0.0`).

**Or** (`"or"`) or (`"||"`)

Pops the top two values of the stack, pushes true (`1.0`) if either was true, else false (`0.0`).

**And** (`"and"`) or (`"&&"`)

Pops the top two values of the stack, pushes true (`1.0`) if both were true, else false (`0.0`).

**Nor** (`"nor"`) or (`"!|"`)

Pops the top two values of the stack, pushes true (`1.0`) if neither were true, else false (`0.0`).

**Nand** (`"nand"`) or (`"!&"`)

Pops the top two values of the stack, pushes true (`1.0`) if both were not true, else false (`0.0`).

**Xor** (`"xor"`) or (`"x|"`)

Pops the top two values of the stack, pushes true (`1.0`) if exactly one was true, else false (`0.0`).

### Stack Commands

**Popping from the stack**(`"pop"`)

Pops (removes) the top value from the stack.

**Duplicating A Value** (`"dup"`)

Pushes a copy of the value on the top of the stack.

### If Statements

*Note: Nesting of if statements is currently not supported by the parser*

If statements can be built with the following three commands: `"if"`, `"else"`, `"endif"`. 'Else' branches are not required and can be omitted. When an `"if"` command is seen, the parser checks the top of the stack, **popping the value** in the process, if the value was true (i.e, not `0.0`) then the code carries on as normal until an `"else"` or `"endif"` command is seen, which ever comes first. If an `"else"` is seen while the condition was true, subsequent commands will be skipped until an `"endif"` is seen, after which execution returns to normal. If the condition was false, then the behavior is the reverse, all of the commands between the `"if"` and `"else"` are skipped.

### Custom Functions

Currently, custom functions have to take exactly two arguments (floats), namely the top two elements of the stack (first argument being the element lower on the stack), and returns a float. These two argument values will be removed in the process (much like the built in commands), and the return value will be pushed to the stack. If you want the parameters to stay on the stack after the function, consider using the `dup` command. If you don't need the return value (or your function returns a dummy value), it can be removed with the `pop` command.

Due to these restricting on the type signature of custom functions, you probably want to uses these functions as wrappers for other functions that take different numbers or types of parameters. Note that you can write (or read) values to the register banks and safely read them in these custom/wrapper functions as needed (e.g. using them as parameters to the function being wrapped). This wrapper might also do type casting or conversion as necessary.

#### **Accessing Registers**

The 3 register banks, R, S, and T each have 10 registers. The can be modified and read by the device outside of the scripts.

**Reading**

`float load_reg(int b, int r)` Returns the value in the rth register of the R (b=0), S (b=1), or T (b=2) bank.

**Writing**

`void store_reg(int b, int r, float val)` Sets the value in the rth register of the R (b=0), S (b=1), or T (b=2) bank to be `val`.
