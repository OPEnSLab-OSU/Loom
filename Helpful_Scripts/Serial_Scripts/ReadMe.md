# Serial Scripts 

These scripts and commands are for accessing/interacting with the Arduino device's serial port without needing to use the Arduino IDE. 

The scripts and commands are designed for use with a Unix (Mac / Linux) terminal.

### Displaying Serial Output

- Check which port to connect to:
- Use `ls /dev/cu.usbmodem*` to check what port to send to  
  - This is easiest with no other USB devices plugged in
- Then run `screen <port> <baud_rate>`
  - Where port is the /dev/cu.usbmodem... output from the previous command 
  - And baud_rate is the baud rate is what the Arduino's serial was initiallized to, probably something like 9600 or 115200
- To exit the `screen` display, enter `Ctrl+A`, `Ctrl+K`, then `y` to confirm 

### Bypassing a 'while(!Serial);' 

If all you need to do is get past the 'while(!Serial);' part of your code, and don't need to see the device's output, you can just send a message to the device over serial. 

- Run the script by right-clicking and opening the script file with the Mac Terminal app (or Linux  command line equivalent)
- *or*
- Open the Terminal app
- Then do either of the following:
  - Run the 'continue_serial.sh' provided
    - Navigate to the script (e.g. `cd /Desktop` if you saved it to the desktop)
    - Enter `./continue_serial.sh`
  - Manually check which serial port the Arduino is connected to
    - Use `ls /dev/cu.usbmodem*` to check what port to send to  
      - This is easiest with no other USB devices plugged in
    - If the device is recognized, it should be displayed as /dev/cu.usbmodem... followed by a four or five digit number
    - Run `echo 1 > /dev/cu.usbmodem...` where the usbmodem number matches the output of the above command 