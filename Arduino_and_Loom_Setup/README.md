# Arduino and Loom Setup Guide

## IDE Setup

### **Install the Arduino IDE** 

- Get and install the [Arduino IDE](https://www.arduino.cc/en/Main/Software)
- Follow Adafruit's [instructions](https://learn.adafruit.com/adafruit-feather-m0-basic-proto/overview) on how to:
  - Add the Adafruit boards index
  - Install Adafruit boards
    - Make sure to install the Adafruit SAMD Boards
  - Install drivers if on Windows 7

### **Download the Loom Library from GitHub**

- Go to the [InternetOfAg](https://github.com/OPEnSLab-OSU/InternetOfAg) folder on GitHub (one directory above this one)

- Click the green 'Clone or Download' button, and 'Download Zip'

- Unzip the downloaded file

### **Install the Loom Dependencies**

The Loom dependecies are provided [here](https://github.com/OPEnSLab-OSU/InternetOfAg/tree/master/Dependencies)

**Mac/Linux**

Either: *(assuming InternetOfAg folder was downloaded to Downloads folder and is unzipped)*

- Find the 'extract_libraries.sh' script from the 'Arduino_and_Loom_Setup' folder from the InternetOfAg download
- Open Terminal
- Drag the script onto the Terminal window
- Press Enter

Or:

- Find the 'Dependencies' folder in the InternetOfAg folder
- Select all of the zip folders
- Right-click and 'Open With' an installed zip extracting application (like Unarchiver)
- Once all of the zip files have been extracted as folders, drag them all to the folder: `Documents/Arduino/libraries ` 

**Windows**

- Find the 'Dependencies' folder in the InternetOfAg folder
- Select all of the zip folders
- Right-click and use 7zip or equivalent to extract the folders
- Once all of the zip files have been extracted as folders, drag them all to the folder: `Documents/Arduino/libraries ` 

### Other Libraries

Install other libraries with the Arduino IDE, as described [here](https://www.arduino.cc/en/guide/libraries)

### **Optional Configuration**

#### **Using an External Editor**

The Arduino IDE editor is a bit limited, so if you want to use a different editor to modify your code, open the Arduino IDE preferences and check the 'Use External Editor' checkbox. This puts a blue tint over the code, and prevents it from being edited in the IDE. Instead, open up your preferred editor to modify your code - anytime you save in that editor, the code in the IDE should update.

**Note –** you will still need to use the IDE to compile and upload your code to the devices 

[Sublime Text](https://www.sublimetext.com) is a great editor you might consider.



## Loom Setup and Usage

**Opening the Code**

Open the `Loom_Library.ino` file from the Loom_Library folder (note that the Arduino IDE requires that any .ino files be in a folder of the same name). This currently opens all of the files of the Loom Library, but there are only two that are of primary concern.

- The .ino file -- this is where you will define the high-level behavior of your device 

- The config.h file -- this is where you will define the hardware your device uses and various behavior options.

*Both files can be renamed, but the .ino file has to match the name of the name of the folder it is within, and the config.h file name should match what is on line 8 of the .ino file (e.g. the '#include "config.h"')*

**Using the Configuration File (config.h)**

- Read the description at the top of the configuration file to understand how it is used
- To start, use the top section of the file to define you best match of your hardware, or some subset (if trying to narrow down issues)

All options have a description to the right

Try to compile your program (check mark icon at the top left of the Arduino IDE)

- If the code does not compile, the reason may be:
  - That you don't have all of the relevant libraries (which shouldn't be the case if you installed all of the libraries following the instructions above)
  - The are some conflicting options set in the configuration file

**Modifying the .ino functionality**

It is recommeded that you base your code off the example provided / use the High-Level API Functions listed at the bottom of the .ino file to for more basic Loom setups. The `loom_interface.h` file describes each of those functions in further detail. 



## Building and Uploading Code Without The IDE

Work is currently be done on investigating and implementing a script that will build the config.h file from the command line, taking the options as arguments. A corresponding Max interface will be made to provide a GUI to these options. The interface will pair with the one to subsequently build and upload code to devices.

The design is planned to be as follows:

- Generate the config file from command line 
  - Arguments to used set options, have default values if none specified
  - Sent to a Python script, inserting the parameters in the relevant locations of the config file
- Be able to call this script from Max
- Use Max to create a graphical interface of toggles / inputs corresponding to the script arguments.
  - Error checking (i.e., of selection conflicts) can be performed in the script or Max, but would doing so in Max would give users immediate feedback on invalid combinations