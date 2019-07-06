

# Loom Quick Start

## Table of Contents

- [Setup Arduino](#setup-arduino)
- [Add Loom Library](#add-loom-library)
- [Run an Example](#run-an-example)
- [Usage](#usage)
- [Adapting Examples](#adapting-examples)
- [Contributing  to Loom](#contributing-to-loom)

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

####Including configuration 

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

```
Manager.parse_config(json_config);
```

#### Print Configuration

If you would like to see the configuration of the device, you can print it with:

```
Manager.print_config();
```

#### Measure Data

#### Package Data

#### Publish to Internet

#### Command Routing

#### More

See the [LoomManager Documentation](http://web.engr.oregonstate.edu/~goertzel/Loom_documentation/class_loom_manager.html) for all avaible `LoomManager` methods.

### Interrupt Manager

### Sleep Manager

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

In some cases behavior might work fine but as the two objects are different, changing one will not change the other

#### Available Methods

See the [Documentation](http://web.engr.oregonstate.edu/~goertzel/Loom_documentation/inherits.html) for available methods of individual modules.

### Printing to Serial

Loom provides a few macros (LPrint, LPrintln, LPrintlnAll) to simplify printing to the Serial monitor. All three take arbitrary many arguments and the arguments can be of any type. This avoids the need for a series of `Serial.print`s.

**LPrint:** Print variable number of arguments (no newline)

```cpp
LPrint("abc", 123);
```

Produces:

```cpp
abc123
```

**LPrintln:** Print variable number of arguments with newline after last element

```cpp
LPrintln("abc", 123);
```

Produces:

```cpp
abc123
```

**LPrintlnAll:** Print variable number of arguments with newline after each element

```cpp
LPrintlnAll(1, 2.34, true, "string", String("another string"));
```

Produces:

```cpp
1
2.34
1
string
another string
```

## Documentation

[Wiki](https://wiki.open-sensing.org/doku.php?id=loom)

[Doxygen generated documentation](http://web.engr.oregonstate.edu/~goertzel/Loom_documentation/)

## Contribute to Loom

Is there a feature you would like to added to Loom? Follow our [guide to contribute to Loom](https://github.com/OPEnSLab-OSU/Loom/Readme_Contributing.md)

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

