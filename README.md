<p align="center">
    <img src="https://github.com/OPEnSLab-OSU/Loom/blob/gh-pages/Aux/OPEnSLogo.png" alt="logo" width="100" height="100">
  </a>
</p>

<h3 align="center">Loom</h3>
<p align="center">
  An Internet of Things Rapid Prototyping System for applications in environmental sensing
  <br>
  <a href="http://www.open-sensing.org/project-loom">Project Page</a>
  ·
  <a href="https://openslab-osu.github.io/Loom/html/index.html">Documentation</a>
  ·
  <a href="https://github.com/OPEnSLab-OSU/Loom/wiki">Lab Wiki</a>
  ·
  <a href=https://github.com/OPEnSLab-OSU/Loom/wiki/Quick-Start>Quick Start</a>
   ·
  <a href=https://github.com/OPEnSLab-OSU/Loom/wiki/Configuration>Configuration</a>
</p>

[![DOI](https://zenodo.org/badge/DOI/10.5281/zenodo.4012710.svg)](https://doi.org/10.5281/zenodo.4012710)

## Table of Contents

- [Table of Contents](#table-of-contents)
- [Introduction](#introduction)
- [Objectives](#objectives)
- [Key Features](#key-features)
- [Installing Arduino and Loom](#installing-arduino-and-loom)
- [Quick Start](#quick-start)
- [Common Pin Allocations](#common-pin-allocations)
- [Hardware Support](#hardware-support)
- [Documentation](#documentation)
- [Contributing](#contributing)
- [Versioning](#versioning)
- [Resources](#resources)
    - [Custom/Modified Libraries used by Loom](#custommodified-libraries-used-by-loom)
- [License](#license)

## Introduction

Loom is an ongoing multidisciplinary collaboration of the OPEnS Lab with a team of 20+ Computer Science and Electrical Engineering students to create a fully open source, modular, user friendly, sensor/actuator Arduino library and ecosystem. The project enables environmental research and conservation communities to overcome significant technical hurdles for creating new environmental, agricultural, and ecological instrumentation to measure, monitor, automate, and understand our world. 

## Objectives

- Design a "plug and play" sensor/actuator system
- Simple enough for K-12 students and non-technical users to use
- Extensible and programmable enough for engineers to customize
- Create a wide variety of applications by simply connecting modular components
- Make the system wireless, low-power, low latency
- Configure all sensors and actuators on a wireless network using an intuitive graphical user interface
- Interact with data and control signals on a network in realtime
- Make data transmitted from local and remote locations available instantly from anywhere around the world

## Key Features

- Quick to setup an entirely new project
- Code contained in an Arduino library
- System behavior defined with a configuration specification and minimal code
- Extensive customizability for complex applications
- Designed for extensibility
- Variety of sensors and actuators supported 
- Remote data logging to SD and internet in near real time

## Installing Arduino and Loom

[Installing Arduino and Loom Quick Setup](https://github.com/OPEnSLab-OSU/Loom/wiki/Arduino-and-Loom-Quick-Setup)

[Installing Arduino and Loom Manual Setup](https://github.com/OPEnSLab-OSU/Loom/wiki/Arduino-and-Loom-Manual-Setup)

## Quick Start

 [Quick Start Guide](https://github.com/OPEnSLab-OSU/Loom/wiki/Quick-Start)

## Common Pin Allocations

[Common Pin Allocations](https://github.com/OPEnSLab-OSU/Loom/wiki/Common-Pin-Allocations)

## Hardware Support

[Hardware Support](https://github.com/OPEnSLab-OSU/Loom/wiki/Hardware-Support)

## Documentation

[Wiki](https://wiki.open-sensing.org/doku.php?id=loom) for general documentation not on GitHub or in Doxygen document

[Doxygen generated documentation Version 1](https://openslab-osu.github.io/Loom/html/index.html) for more verbose documentation that is designed for developers

[Doxygen generated documentation Version 2](https://openslab-osu.github.io/Loom/index.html) for more user-friendly documenation that is designed for end-users


***Note***: The documentation does not currently update automatically, if would want to be sure you have the most up to date version of the documentation, you can run Doxygen to manually generate a local copy of the documentation. See the Doxygen website on how to [Install Doxygen](http://www.doxygen.nl/manual/install.html) and [Doxygen Usage](http://www.doxygen.nl/manual/doxygen_usage.html). 

Once installed, the run `doxygen Doxyfile` from the command line (from Loom directory) to generate the documentation. To view, open the `Loom/html/index.html` file in a browser.

## Contributing

Is there hardware support or other feature you would like to add to Loom? Follow our [Guide to contributing to Loom](https://github.com/OPEnSLab-OSU/Loom/wiki/Contributing-to-Loom)

## Versioning

Loom aims to follow [Semantic Versioning](https://semver.org) 

## Resources

- [Project Page](http://www.open-sensing.org/project-loom) for Loom on OPEnS lab website
- [GitHub Organization](https://github.com/OPEnSLab-OSU) for all OPEnS Lab projects
- [Loomify](https://github.com/OPEnSLab-OSU/Loomify) An npm package to drive the backend Loom Tag Format parsing and Loom Library github interactions.
- [Loom Configurator](https://github.com/OPEnSLab-OSU/Loom_Configurator) A WebApp designed for designing and exporting loom configurations
- [Loom-Network](https://github.com/OPEnSLab-OSU/Loom-Network) Arduino networking library designed for use with any wireless radio
- [Loom-Auxilliary](https://github.com/OPEnSLab-OSU/Loom_Auxiliary) General Loom and lab non-code files
- [Max-Loom2](https://github.com/OPEnSLab-OSU/Max-Loom2) MaxMSP interfaces for Loom interactivity

#### Custom/Modified Libraries used by Loom


- [OPEnS_RTC](https://github.com/OPEnSLab-OSU/OPEnS_RTC) A modified arduino library for with support for a variety of RTCs
- [SSLClient](https://github.com/OPEnSLab-OSU/SSLClient) Arduino library to add SSL functionality to any Client class 
- [EthernetLarge](https://github.com/OPEnSLab-OSU/EthernetLarge) Ethernet Library for Arduino, modified to support larger buffers for SSLClient

## License 

Loom is licensed under [GNU General Public License v3.0](https://github.com/OPEnSLab-OSU/Loom/blob/master/LICENSE)
