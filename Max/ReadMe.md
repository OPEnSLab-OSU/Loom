# Project Loom: Max Interfaces

These are the Max/MSP interfaces for the control, monitoring, and processing of devices and data of a Loom network. 

Each module contains a set of related functions and can receive, process, or output streams of data. Modules can be linked together to pass information from one to another. The set of data processor plugins include various means of reading any information from a network, a variety of modules to process, convert, or display this information, and modules to send data back out to devices on the network. 

As more hardware is added or specialized needs arise, users can make modules to provide relevant displays, processing, and I/O, tailored to their needs.

## Table of Contents

1. [Max/MSP Setup](#max/msp-setup)
    1. [Max Installation](#max-installation)
    2. [Loom Data Processors Setup](#loom-data-processors-setup)
2. [Existing Max Patches](#existing-max-patches)
    1. [Input](#input)
    2. [Output](#output)
    3. [Processing](#processing)
    4. [Monitors](#monitors)
3. [Running Max Interfaces](#running-max-interfaces)
4. [Channel Manager](#channel-manager)
5. [Developing New Max Patches](#developing-new-max-interfaces)
    1. [Modifying Existing Patches](#modifying-existing-patches) 
    2. [Developing Patches from Scratch](#developing-patches-from-scratch) 
    3. [Developing Patches using the Core](#developing-patches-using-the-core)
    4. [Adding a Patch as an option to ProcessorGen](#adding-as-option-to-processorgen) 
    5. [Dynamic Window Size](#dynamic-window-size) 
6. [Troubleshooting](#troubleshooting)

## Max/MSP Setup

### Max Installation

- Download Max from [Cycling '74](https://cycling74.com/downloads)
- Install Max with downloaded installer
- If you plan on using 32-bit peripherals with Max 
  - Download and install 32-bit legacy Java (if not already installed)
    - For [Mac users](https://support.apple.com/kb/dl1572?locale=en_US)
    - For [Windows / other users](https://java.com/en/download/manual.jsp)
  - May need to force 32-bit mode
    - Mac: In Finder, navigate to Application > Max; press Cmd+I; Check "Open in 32-bit mode"
- Activate Max
  - In Max, go to Help Menu > User Account and Licenses 
  - Create a Cycling '74 account if you haven't already
  - Put in activation code

### Loom Data Processors Setup

The Data Processor Plugins folder needs to be placed in the Max library in order to properly function. The specific location is as follows:

**Max 7:**  [user] / Documents / Max 7 / Library

**Max 6:**  [user] / Applications / Max6 / Cycling74 / 

***Note:***  DataProcessor Plugins currently **Must** be in the same folder as the master DataProcessor.maxpat file and processor.js file

## Existing Max Patches

Loom provides a number of patches for monitoring, receiving, sending, processing, and displaying information of the devices connected to the LOOM network. Current Max patches are the following:

### Network Input 

- Arduino In
- Emote
- LOOM In
- Multiplexer Monitor
- MIDI In
- OSC In

### Processing

- 3D Pan
- Chord Degree
- Concatenate
- Drum Kit
- Movement
- Musicbox
- Range Setter
- Rotary Dial
- Scale Curve
- Scale Degree
- Scale Linear
- Sequencer
- Simple Sample
- Smooth
- Spring
- Threshold
- Tap Tempo

### Network Output

- App 3D Pan 
- App VST
- DMX
- Instrument Position
- MIDI Out
- Neopixel 
- OSC Out
- Out Lightsaber
- Relay
- Servo
- Stepper Motor
- Switchblade

### Monitors

- Ishield Monitor
- LOOM Channel Manager

### Other

- Stepper Motor Extension
- CSV writer

## Running Max Module Interfaces

The simplest way to test or run the devices on your network is to open the DataProcessor.maxpat file. This patch contains a drop down menu of all the Loom Max modules / patches, and selecting one will generate an instance of it. Multiple instances can exist of the same Max patch, each controlling or monitoring different data flow. This Data Processor also already contains an instance of the Loom Channel Manager, described below. One can also add normal, non-prebuilt Max objects to the Data Processor like any other .maxpat file

## Channel Manager 

The Loom Channel Manager keeps track of the which devices are on using which channel (A – H) on the specified Family and Family Subnet (number), whether they are currently active, their battery levels, and allows for the following reconfiguration options:

- **Save Config:** Save any changes that have been made to the device. Without saving, restarting the device will start in its previously saved state (which may be desirable if making temporary changes)
- **Set Request Settings:** Enables a flag on the given device, instructing it to, on startup, ignore any stored channel and instead request a new one from a channel manager (which needs to be open for any automatic channel assignment to occur). Note that a 'Save Config' command needs to be sent to the device for the set to take effect.
- **Reassign Channel:**  Changes the channel of a given running device to a specified available channel. Note that this will not persist after device restart if no 'Save Config' command is sent.
- **Remove:** Disconnects the specified device from the network. It will switch WiFi to AP mode. Note that this will not persist after device restart if no 'Save Config' command is sent.

Devices that go to sleep will be remembered (and thus preventing that channel from being auto-assigned to another device) unless 'Clear Memory' is pressed. For devices with buttons, pressing the button will light an indicator next to the corresponding device and channel information.

## Developing New Max Patches

Users are free to modify existing and create new Max patches to achieve the desired functionality.

### Modifying Existing Patches

Open the patch you wish to modify (recommeded that you modify a copy of the patch in question) and enter Edit mode. The layout of the patch can be modified in the Presentation view and functionality in Edit mode.

### Developing Patches from Scratch

Refer the the Max Help, Reference, and Examples which can be found under the Help menu

Some patchers in existing patches are self contained and can simply be copied into a new patch to add the functionality as a tab. The 'WiFi config' tab / patcher is an example of this. Any other subpatch can also be set to appear as a tab by setting it to do so in the Inspector.

### Developing Patches using the Core

The majority of Loom-based patches are now implemented through a 'Core' with several layers, but ultimately allows any of the patches that interface with WiFi devices to use a common base. The best way to see this is to open an existing sensor monitor or actuator controller, or the Template.maxpat. Each one only has a few unique elements, namely those relating to the interface.

To implement a new patch based on the core use a copy of the Template or one of the existing patches, if it is alreadly similar to what you want. Add/modify the interface to support what you need and make sure that any outgoing commands you want are routed into the Core for processing in the Javascript code. The Javascript may need updating to be able to parse new commands, as the first element of a list sent to the JS controller is expected to be the name of the function you want to run.

### Adding a Patch as an option to ProcessorGen

Follow these steps if you would like to be able to access a custom patch from the dropdown menu in the DataProcessors / ProcessorGen.

- Place custom patch into DataProcessorPlugins folder
- Open processorGen.maxpat in the DataProcessorPlugins folder
- Switch to Edit Mode: 
  - View > Edit (Mac: Cmd+E, Windows: Ctrl+E)
- Select Dropdown umenu object
- Open Inspector Window
  - View > Inspector Window (Mac: Cmd+Shift+I, Windows: Ctrl+Shift+I)
- Find the Menu Items parameter under Items section, and click Edit
- Add the name of your custom patch to the list (does not have to be the last item)

### Dynamic Window Size

If your custom patch does not fit inside the standard 250x150 dimensions that most LOOM patches are sized at, you may want to adjust the size of the border around the generated instance of a patch to avoid having to resized it manually. The process of doing this is as follows:

- Open processor.js in the DataProcessorPlugins folder

- Locate the createBpatcher function

- In the ```switch(file)``` add another case in the same format, using the name and dimensions of your custom patch 

## Troubleshooting

- **JVM Errors:** 32-bit support may be needed, check setup section for details