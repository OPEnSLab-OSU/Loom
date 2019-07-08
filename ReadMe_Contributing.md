# Contributing to Loom

## Table of Contents

- [Create Branch or Fork](#create-branch-or-fork)
- [Creating a Module Class](#creating-a-module-class)
- [Documenting Additions](#documenting-additions)

- [Creating an Example](#creating-an-example)
- [Making a Pull Request](#making-a-pull-request)

## Create Branch or Fork



## Creating a Module Class

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

### Implementing Module

https://github.com/OPEnSLab-OSU/Loom/blob/master/Readme_Documentation_Style_Guide.md

#### Constructor

### Code that Needs to be Updated

While Loom aims to keep the number of locations in code that need to be editted which the addition of a new module to a minumum, there are a few locations outside of your new module files that will need to be updated.

### Support for Executing Commands



## Documenting Additions

Follow our [style guide](https://github.com/OPEnSLab-OSU/Loom/blob/master/Readme_Documentation_Style_Guide.md) for documentation and ensuring that any new module classes appear as options in the configurator.

## Creating an Example



## Making a Pull Request