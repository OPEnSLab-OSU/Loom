# Project LOOM: Max Interfaces

These are the interfaces for the control, montoring, and processing of devices and data of a LOOM network.

Each module contains a set of similar functions and can receive and/or output streams of data. Modules can be linked together to pass information from one to another. The set of data processor plugins include various means of reading any information from a network, a variety of modules to process, convert, or display this information, and modules to send data back out to the network. 

As more hardware or specialized needs arise, users can make modules to provide relevant displays, processing, and I/O, tailored to their needs.

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

- **Arduino In**
- **Emote**
- **LOOM In**
- **Multiplexer Monitor**
- **MIDI In**
- **OSC In**

### Processing

- **3D Pan**
- **Chord Degree**
- **Concatenate**
- **Drum Kit**
- **Movement**
- **Musicbox**
- **Range Setter**
- **Rotary Dial**
- **Scale Curve**
- **Scale Degree**
- **Scale Linear**
- **Sequencer**
- **Simple Sample**
- **Smooth**
- **Spring**
- **Threshold**
- **Tap Tempo**

### Output

- **App 3D Pan** 
- **App VST**
- **DMX**
- **Instrument Position**
- **MIDI Out**
- **Neopixel** – Set RGB values of a Neopixel(s) connected to an Ishield. Can be daisy chained
- **OSC Out**
- **Out Lightsaber**
- **Relay** – Toggle state of relay on pin 5 or 6
- **Servo** – Set specified servo to provided degree
- **Switchblade**

### Monitoring

- **Ishield Monitor**
- **LOOM Channel Manager**

## Developing New Max Patches

Users are free to modify existing and create new match patches to achieve the desired functionality.

### Modifying Existing Patches

Open the patch you wish to modify (recommeded that you modify a copy of the patch in question) and enter Edit mode. The layout of the patch can be modified in the Presentation view and functionality in the 

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

If your custom patch does not fit inside the standard 250x150 dimensions that most LOOM patches use, you may want to adjust the size of the patch generated to avoid having to resized it manually. The process of doing this is as follows:

- Open processor.js in the DataProcessorPlugins folder
- Locate the createBpatcher function
- In the ```switch(file)``` add another case in the same format, using the dimensions of your custom patch 

## Troubleshooting

- **JVM Errors:** 32-bit support may be needed, check setup section for details