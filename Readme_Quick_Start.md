

# Loom Quick Start

This guide is intended to help users with the typical usage of Loom.

## Table of Contents

- [Setup Arduino](#setup-arduino)
- [Add Loom Library](#add-loom-library)
- [Run an Example](#run-an-example)
- [Usage](#usage)
- [Adapting Examples](#adapting-examples)
- [Contributing  to Loom](#contributing-to-loom)
- [Basic Example](#basic-example)

## Setup Arduino

If you haven't already, set up Arduino following our [IDE Setup Guide](https://wiki.open-sensing.org/doku.php?id=ide-setup).

## Install Loom Library 

Get the Loom code from here on [GitHub](https://github.com/OPEnSLab-OSU/Loom) (the large green button)

- If you intend to contribute to Loom you will probably want to clone the library
- Otherwises you can just download the zip folder
  - Use the Arduino IDE > Sketch > Include Library > Add .ZIP Library… to add the downloaded library

## Run an Example

Start by compiling on of the Loom examples (File > Examples > Loom >) to ensure that it compiles. If not, review the previous steps.

## Usage

### Configuration

The configuration of a device is specified via a JSON configuration. In examples, this takes the form of a config.h file. See our [JsonConfigExample](https://github.com/OPEnSLab-OSU/Loom/tree/master/examples/JsonConfigExample) on how you can define a configuration. Note that there are a few different ways to define the configuration:

- In .ino file
- External file, compact
- External file, pretty

The external .h file options are preferred, either of these two versions is fine, but the "pretty" format takes up more space, so for deployments, prefer "compact".

See the [documentation](http://web.engr.oregonstate.edu/~goertzel/Loom_documentation/) or source code (.h files) for valid module parameter.

#### Including configuration 

As shown in the example, include a configuration like so:
```cpp
const char* json_config = 
#include "json_config.h"
;
```

*Note that these 3 lines **cannot** be shortened into one*

#### Configurator Interface

Work is being done to build an interface that allows users to graphically specify a configuration.h file that you can place in the sketch folder. See [Loom Configurator](https://github.com/OPEnSLab-OSU/Loom_Configurator) and [Loomify](https://github.com/OPEnSLab-OSU/Loomify) for the development of this web app.

### Loom Manager 

The `LoomManager` automates much of the operation of a Loom device, preventing you from having to individually manager each module. The most important features of the Manager are covered here.

#### Parsing Configuration

In order to use a Json configuration, it needs to be parsed by a `LoomManager`. For each component in the configuration, the manager will create a module according to the provided settings. Pass the configuration to the manager in `setup())` as follows (see bottom of this readme for full program):

```cpp
Manager.parse_config(json_config);
```

#### Print Configuration

If you would like to see the configuration of the device, you can print it with:

```cpp
Manager.print_config();
```

#### Measure Data

The manager provides a `measure()` method to automate the process of measuring data of all sensor type modules. The method does not take an arguments or return anything.

```cpp
Manager.measure();
```

#### Package Data

Packaging data entails copying the measured data into a Json object. Non-sensor type modules may also package data to the Json, depending on `package_verbosity` settings. Use either of the following format to extract the packaged data from the manager.

```cpp
JsonObject data1 = Manager.package();
// or
JsonObject data2;
Manager.package(data2);
```

Note that you can also get the current state of the `LoomManager`s internal Json object at any time (without updating the data that is packaged) with `JsonObject internal_json (bool clear=true)`, which can be used as follows. By default this operation empties the Json object (so you can use it), if you do not want to clear the data, send argument false to the method.

```cpp
JsonObject notClearedJson = Manager.internal_json(false);

JsonObject clearedJson = Manager.internal_json();
```

#### Append to Data Json

After package data, you can add non-Loom data to the Json data package with `template<typename T> bool add_data(const char* module, const char* key, T val)`.

**Example:**

```
Manager.add_data("exampleModule", "key", 12345);
```

You can also add to a Loom module's section of the data object. If the provided module name matches a component already in the data package, your data key value pair will be added to that component.

#### Print Json object

If you want to print the Json object to the Serial monitor, the ArduinoJson library provides built in functionality to do so, formatting it to be easily readable.

```cpp
JsonObject toPrint;
serializeJsonPretty(toPrint, Serial);
```

#### Publish to Internet

#### Command Routing

Some module support executing commands based on a command defined in a Json object. Rather than send the command directly to the intended module, you can just send it to the `LoomManager`, which will determine which module it goes to and forward the command to it.

```
JsonObject command;
Manager.cmd_route(command);
```

For more details on executing commands and how the Json of a command is structured, see the [Executing Commands](https://github.com/OPEnSLab-OSU/Loom/blob/master/Readme_Executing_Commands.md) readme.

#### More

See the [LoomManager Documentation](http://web.engr.oregonstate.edu/~goertzel/Loom_documentation/class_loom_manager.html) for all avaible `LoomManager` methods.

### Module

All modules within Loom inherit from `LoomModule` and have access to any public `LoomModule` methods.  The same applies for the rest of the inheritance hierarchy. For example, any sensor (directly or indirectly) inherits from `LoomSensor` and thus has access to `LoomSensor` public methods. Inherited methods are shown in the documentation.

The methods of modules managed by a LoomManager can be access with dot syntax. 

For example if you wanted to access a relay module being managed by a `LoomManager`, you can access the `LoomRelay` module via:

```cpp
// Manager is a LoomManager
Manager.Relay();
```

The above command does not actually do anything, but it gives you access to the methods of the `LoomRelay`. For example, to set the relay, `LoomRelay` has a method `void set(bool enable)`, which we can access like:

```cpp
Manager.Relay().set(true);
```

You can also set a variable to the return of a method like:

```cpp
bool moduleActive = Manager.Relay().get_active();
```

If you have multiple of a given type of module, you can access them via index (zero indexed):

```cpp
Manager.Relay(2); // get third relay
```

**Does not exist:** If any time you try to access a module that does not exist, rather than receiving a null pointer, you will just get a `LoomModuleStub` that will simply print out an error for any method you try to run on it.

**Setting module access to variable:** Note that while you can technically set the module accessed to a variable, it is recommended against. For example:

```cpp
Loom_Relay relay = Manager.Relay();
// relay and Manger.Relay() now refer to different objects
```

In some cases behavior might work fine but as the two objects are different, changing one will not change the other.

#### Available Methods

See the [Documentation](http://web.engr.oregonstate.edu/~goertzel/Loom_documentation/inherits.html) for available methods of individual modules.

### Secondary Managers

Loom has a few secondary managers that are `LoomModules` specialized to managing features more general than an individual hardware component. These currently include:

- Interrupt manager
- Sleep manager
- Multiplexer

See the [Secondary Managers](https://github.com/OPEnSLab-OSU/Loom/blob/master/Readme_Secondary_Managers.md) readme for details on their usage.

### Printing to Serial

Loom provides a few macros (LPrint, LPrintln, LPrintlnAll) to simplify printing to the Serial monitor. All three take arbitrary many arguments and the arguments can be of any type. This avoids the need for a series of `Serial.print`s.

**LPrint:** Print variable number of arguments (no newline)

```cpp
LPrint("abc", 123);
// produces:
// abc123
```

**LPrintln:** Print variable number of arguments with newline after last element

```cpp
LPrintln("abc", 123);
// produces:
// abc123
```

**LPrintlnAll:** Print variable number of arguments with newline after each element

```cpp
LPrintlnAll(1, 2.34, true, "string", String("another string"));
// produces:
// 1
// 2.34
// 1
// string
// another string
```

## Documentation

[Wiki](https://wiki.open-sensing.org/doku.php?id=loom) for general documentation not contained in GitHub readmes

[Doxygen generated documentation](http://web.engr.oregonstate.edu/~goertzel/Loom_documentation/) for documentation of the code and how to use

## Contribute to Loom

Is there hardware support or other feature you would like to add to Loom? Follow our [guide to contribute to Loom](https://github.com/OPEnSLab-OSU/Loom/Readme_Contributing.md)

## Basic Example

The code for this example can also be found on [GitHub](https://github.com/OPEnSLab-OSU/Loom/tree/master/examples/BasicExample) or via the Arduino IDE (File > Examples > Loom > Basic) if you have Loom installed. 

This is a basic example of specifying a configuration, including it, using the manager, parsing a config, and printing out analog data readings.

```cpp
#include <Loom.h>

// Include configuration
const char* json_config = 
#include "config.h"
;

LoomManager Manager("Manager", "Loom", 1, 1, LoomManager::DeviceType::NODE, Verbosity::V_HIGH, Verbosity::V_LOW);

void setup() 
{
  pinMode(LED_BUILTIN, OUTPUT); 

  Serial.begin(115200);
  while(!Serial);
  delay(1000);

  LPrintln("Initialized Serial!\n");

  Manager.parse_config(json_config);
  Manager.print_config();

  LPrintln("\n ** Setup Complete ** ");
}

void loop() 
{
  Manager.measure();
  JsonObject tmp = Manager.package();
  serializeJsonPretty(tmp, Serial);

  delay(2000);
}
```

