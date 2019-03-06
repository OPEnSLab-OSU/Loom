# Project Loom: Extending Loom

New sensor and actuator support can be added by filling out the loom_mod_template_sensor.h or loom_mod_template_actuator.h file with relevant code. This template ensures that the code for each piece of hardware conforms to a standard format, making it easier to read and understand.

New files should be added #included in the loom_preamble.h file after config.h but generally before loom_flash.h and loom_common.h.

There are plans/consideration for a script that will handle most of the additions for a user based on various provided parameters / needs as current additions generally need to modify the config, preamble, loom_commom (sometimes), and loom_interface files in a mostly boiler plate fashion in addition to the new device-specific file.

#### Adding Custom Functions to Message Router

There are a few steps that must be followed in order to get a custom function to be callable by the message router. These are:

- Write your function in the correct format
- Add the function prototype to the top of loom_msg_router.h file
- Add the dispatch call to the msg_router function

**Writing Your Function**

All function that are called by the message router need to take only an OSC message by reference, and return nothing. Thus a function skeleton would look like:

```
void your_function(OSCMessage &msg)
{
	// process the message forwarded from msg_router
}
```

Inside this function is generally where you will process the message (if you need, you can add auxiliary function(s), to do some of the processing/handling - these do not have constraints on the parameters or return value). The following OSC methods are recommended for extracting the arguments from the message. (The below list is not exhaustive, see the [OSC API](https://github.com/CNMAT/OSC/blob/master/API.md) for more functions)

**`msg.size()`** Returns the number of arguments in the the message

**`msg.getType(index)`** Returns the type of the argument at `index`, `i` for integer, `f` for float, and `s` for string. Useful in combination with switch statement to select which of the following three functions to call.

**`msg.getInt(index)`** Returns argument to message that is an integer at `index` if argument is indeed an integer and there is an argument at that index

**`msg.getFloat(index)`** Returns argument to message that is a float at `index` if argument is indeed a float and there is an argument at that index

**`msg.getString(index)`** Returns argument to message that is a string at `index` if argument is indeed a string and there is an argument at that index

**`msg.getAddress(buf, 0);`** Copies address of message into char array called `buf`

**Adding the Function Prototype**

Add the protype of the function you wrote, e.g:

```
void your_function(OSCMessage &msg);
```

to the `Custom Function Prototypes` section at the top of the `loom_msg_router.h` file. (Don't forget the semicolon). Should be no need to add the prototypes for any auxililary functions called by the `your_function`.

**Adding the Dispatch Call**

At the bottom of the message router, add:

```
if (msg.dispatch("/command/string", your_function, addrOffset) ) return;
```

Where

- `/commmand/string` is the section after the device identifying string that is being used to select which function to dispatch a message to. This should be descriptive but not overly long. In the above example,  the string `setID` was used to check if the message should be forwarded to `set_instance_num`
- `your_function` is the name of the function you want to be called if the message matches the device identifying string and command string.
- `addrOffset` should stay as it is, this is an integer value passed in from the `bndl->route(...)` call, and simply specifies where in the message string to start looking for the command section (i.e. right after the device identifying string)
- The return is used to stop checking for other matches once the correct function is dispatched.