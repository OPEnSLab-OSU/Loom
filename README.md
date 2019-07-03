<p align="center">
    <img src="http://web.engr.oregonstate.edu/~goertzel/OPEnSLogo.png" alt="logo" width="100" height="100">
  </a>
</p>

<h3 align="center">Loom</h3>
<p align="center">
  An Internet of Things Rapid Prototyping System for applications in environmental sensing
  <br>
  <a href="http://www.open-sensing.org/project-loom">Project Page</a>
  ·
  <a href="http://web.engr.oregonstate.edu/~goertzel/Loom_documentation_html/">Documentation</a>
  ·
  <a href="https://wiki.open-sensing.org/doku.php?id=loom">Wiki</a>
  ·
  <a href=https://github.com/OPEnSLab-OSU/Loom/Readme_Quick_Start.md>Quick Start</a>
</p>

[![DOI](https://zenodo.org/badge/DOI/10.5281/zenodo.3266419.svg)](https://doi.org/10.5281/zenodo.3266419)

## Table of Contents

- [Introduction](#introduction)
- [Objectives](#objectives)
- [Key Features](#key-features)
- [Quick Start](#quick-start)
- [Hardware Support](#hardware-support)
- [Documentation](#documentation)
- [Contributing](#contributing)
- [Versioning](#versioning)
- [Resources](#resources)
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

## Quick Start

 [Quick Start Guide](https://github.com/OPEnSLab-OSU/Loom/blob/master/Quick_Start_Readme.md)

## Hardware Support

[Hardware Support](https://github.com/OPEnSLab-OSU/Loom/blob/master/Readme_Hardware_Support.md)

## Documentation

[Wiki](https://wiki.open-sensing.org/doku.php?id=loom)

[Doxygen generated documentation](http://web.engr.oregonstate.edu/~goertzel/Loom_documentation/)

## Contributing

[Guide to contributing to Loom](https://github.com/OPEnSLab-OSU/Loom/blob/master/ReadMe_Contributing.md)

## Versioning

Loom aims to follow [Semantic Versioning](https://semver.org) 

## Resources

- [Project Page](http://www.open-sensing.org/project-loom) for Loom on OPEnS lab website
- [GitHub Organization](https://github.com/OPEnSLab-OSU) for all OPEnS Lab projects
- [Loomify](https://github.com/OPEnSLab-OSU/Loomify) An npm package to drive the backend Loom Tag Format parsing and Loom Library github interactions.
- [Loom Configurator](https://github.com/OPEnSLab-OSU/Loom_Configurator) A WebApp designed for designing and exporting loom configurations
- [Loom-Network](https://github.com/OPEnSLab-OSU/Loom-Network) Arduino networking library designed for use with any wireless radio
- [Loom-Auxilliary](https://github.com/OPEnSLab-OSU/Loom_Auxiliary) General Loom and lab non-code files
- [Loom-Max](https://github.com/OPEnSLab-OSU/Loom-Max) MaxMSP interfaces for Loom interactivity

#### Custom/Modified Libraries used by Loom


- [OPEnS_RTC](https://github.com/OPEnSLab-OSU/OPEnS_RTC) A modified arduino library for with support for a variety of RTCs
- [WiFi201](https://github.com/OPEnSLab-OSU/WiFi201) Upgraded WiFi library for the Arduino WiFi Shield 101 and MKR1000 board
- [SSLClient](https://github.com/OPEnSLab-OSU/SSLClient) Arduino library to add SSL functionality to any Client class 
- [EthernetLarge](https://github.com/OPEnSLab-OSU/EthernetLarge) Ethernet Library for Arduino, modified to support larger buffers for SSLClient

## License 

Loom is licensed under [GNU General Public License v3.0](https://github.com/OPEnSLab-OSU/Loom/blob/master/LICENSE)