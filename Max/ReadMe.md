# Project LOOM: Max Interfaces

These are the Max/MSP interfaces for the control, monitoring, and processing of devices and data of a LOOM network.

Each module contains a set of similar functions and can receive, process, and output streams of data. Modules can be linked together to pass information from one to another. The set of data processor plugins include various means of reading any information from a network, a variety of modules to process, convert, or display this information, and modules to send data back out to the network. 

As more hardware is added or specialized needs arise, users can make modules to provide relevant displays, processing, and I/O, tailored to their needs.

## Max/MSP Setup

### Max Installation

- Download Max from [Cycling '74](https://cycling74.com/downloads)
- Install Max with downloaded installer
- If you plan on using 32 bit peripherals with Max 
  - Download and install 32-bit legacy Java (if not already installed)
    - For [Mac users](https://support.apple.com/kb/dl1572?locale=en_US)
    - For [Windows / other users](https://java.com/en/download/manual.jsp)
  - May need to force 32-bit mode
    - Mac: In Finder, navigate to Application > Max; press Cmd+I; Check "Open in 32-bit mode"
- Activate Max
  - In Max, go to Help Menu > User Account and Licenses 
  - Create Cycling '74 account
  - Put in activation code

### LOOM Data Processors Setup

The Data Processor Plugins folder needs to be placed in the Max library in order to properly function. The specific location is as follows:

**Max 7:**  [user] / Documents / Max 7 / Library

**Max 6:**  [user] / Applications / Max6 / Cycling74 / 

***Note:***  DataProcessor Plugins **Must** be in the same folder as the master DataProcessor.maxpat file and processor.js file

## Existing Max Patches

LOOM provides a number of patches for monitoring, receiving, sending, processing, and displaying information of the devices connected to the LOOM network

### Input

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

### Output

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

## Running Max Interfaces

The simplest way to test or run the devices on your network is to open the DataProcessor.maxpat file. This patch contains a drop down menu of all the LOOM Max modules / patches, and selecting one will generate an instance of it. Multiple instances can exist of the same Max patch, each controlling or monitoring different data flow. This Data Processor also already contains an instance of the LOOM Channel Manager, described below.

## Channel Manager 

The LOOM Channel Manager keeps track of the which devices are on using which channel (A – H), whether they are currently active, their battery levels, and allows for the following reconfiguration options:

- **Save Config:** Save any changes that have been made to the device. Without saving, restarting the device will start in its previously saved state (which may be desirable if making temporary changes)
- **Set Request Settings:** Enables a flag on the given device to, on startup, ignore any stored channel and instead request a new one from a channel manager (which needs to be open for any automatic channel assignment to occur). Note that a 'Save Config' command needs to be sent to the device for the set to take effect.
- **Reassign Channel:**  Changes the channel of a given running device to a specified available channel. Note that this will not persist after device restart if no 'Save Config' command is sent.
- **Remove:** Disconnects the specified device from the network. It will switch WiFi to AP mode. Note that this will not persist after device restart if no 'Save Config' command is sent.

## Developing New Max Patches

Users are free to modify existing and create new Max patches to achieve the desired functionality.

### Modifying Existing Patches

Open the patch you wish to modify (recommeded that you modify a copy of the patch in question) and enter Edit mode. The layout of the patch can be modified in the Presentation view and functionality in Edit mode.

### Developing Patches from Scratch

Refer the the Max Help, Reference, and Examples which can be found under the Help menu

### Adding as option to ProcessorGen

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

If your custom patch does not fit inside the standard 250x150 dimensions that most LOOM patches are sized at, you may want to adjust the size of the patch generated to avoid having to resized it manually. The process of doing this is as follows:

- Open processor.js in the DataProcessorPlugins folder
- Locate the createBpatcher function
- In the ```switch(file)``` add another case in the same format, using the dimensions of your custom patch 

## Troubleshooting

- **JVM Errors:** 32-bit support may be needed, check setup section for details