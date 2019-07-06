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

## Add Loom Library 

Get the Loom code from here on [GitHub](https://github.com/OPEnSLab-OSU/Loom) (the large green button)

- If you intend to contribute to Loom you will probably want to clone the library
- Otherwises you can just download the zip folder
  - Use the Arduino IDE > Sketch > Include Library > Add .ZIP Library… to add the downloaded library

## Run an Example

Start by compiling on of the Loom examples (File > Examples > Loom >) to ensure that it compiles. If not, review the previous steps.

## Usage

### Using API

#### Manager 

#### Interrupt Manager

#### Sleep Manager

#### Module

All modules within Loom inherit from `LoomModule` and have access to any public `LoomModule` methods.  The same applies for the rest of the inheritance hierarchy. For example, any sensor (directly or indirectly) inherits from `LoomSensor` and thus has access to `LoomSensor` public methods. Inherited methods are shown in the documentation.

#### Other

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

```
bool moduleActive = Manager.Relay().get_active();
```

If you have multiple of a given type of module, you can access them via index (zero indexed):

```
Manager.Relay(2); // get third relay
```

If any time you try to access a module that does not exist, rather than receiving a null pointer, you will just get a `LoomModuleStub` that will simply print out an error for any method you try to run on it.

See the [Documentation](http://web.engr.oregonstate.edu/~goertzel/Loom_documentation/) for available methods of individual modules.

### Documentation

[Wiki](https://wiki.open-sensing.org/doku.php?id=loom)

[Doxygen generated documentation](http://web.engr.oregonstate.edu/~goertzel/Loom_documentation/)

## Adapting Examples

### Modifing Configurations

The configuration of a device is specified via a JSON configuration. In examples, this takes the form of a config.h file. See our [JsonConfigExample](https://github.com/OPEnSLab-OSU/Loom/tree/master/examples/JsonConfigExample) on how you can define a configuration. 

See the [documentation](http://web.engr.oregonstate.edu/~goertzel/Loom_documentation/) or source code (.h files) for valid module parameter.

Work is being done to build an interface that allows users to graphically specify a configuration.h file that you can place in the sketch folder. See [Loom Configurator](https://github.com/OPEnSLab-OSU/Loom_Configurator) and [Loomify](https://github.com/OPEnSLab-OSU/Loomify) for the development of this web app

## Contribute to Loom

Is there a feature you would like to added to Loom? Follow our [guide to contribute to Loom](https://github.com/OPEnSLab-OSU/Loom/Readme_Contributing.md)