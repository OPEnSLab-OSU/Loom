# PushingBox to Google Sheet Pipeline

## Data Pipeline 
The hub receives data and makes a GET request to a PushingBox Scenario using the data it's trying to send as URL arguments. The PushingBox Scenario then formats this data and uses the formatted data as URL arguments for the GET request it makes of the Google Sheet, which then parses and displays the data.

## Abstraction

The library code, script, and PushingBox components have all been modified and improved to work with arbitrary:

- Spreadsheets
- Sheets / Tabs within a spreadsheet
- Numbers of arguments 
  - *There used to be a limit of 16 key-value pairs, that has since been circumvented*

## Usage

#### Creating Spreadsheets

No extra PushingBox or Google Script modifications need to be made to be able to write to a new spreadsheet. A spreadsheet can be created as normal by creating a new Google Sheet. All that you need now is the spreadsheet ID. This can be found in the spreadsheets URL. An example URL is shown below :

```
https://docs.google.com/spreadsheets/d/1Hv2oME3sjumMXv36GkFV1Q7I83xnXu-f-ZrxULsXS_U/edit#gid=1643186642
```

You need the ID, which is the string of characters after the `.../d/` and before the `/edit#...`, which in this case would be: `1Hv2oME3sjumMXv36GkFV1Q7I83xnXu-f-ZrxULsXS_U`

#### Code

Using the Loom Library, all you need to specify to be able to write to the spreadsheet you just created, is defining `spreadsheet_id` and `tab_id` in the PushingBox section `config.h` file. The spreadsheet ID is the string of characters obtained from the spreadsheet URL. The tab ID is label of the tab/sheet that is written to. This can be a a string or number, but probably should not contain spaces. Specifying the tabID to be one that alread exists in the spreadsheet will result in the data starting at the next available row. If the specified tabID does not yet exist, it will simply be created and the data will start at the top. There are various options in the config file specifying how to create the tab ID (e.g. whether hub or node defines it) and an option to use an array of spreadsheet IDs, in the event that you want each family of devices to get its own spreadsheet.

**Device ID**

Note that there is a device_id in this section as well. This might need to be changed if you have made a new PushingBox scenario (reasons for doing so explained in the next section), otherwise you can likely leave it as is. 

**Upload Filter**

There is also an option to enable an upload filter to prevent uploading data too quickly, by specifying a minimum duration that must elapse between uploads. This does not automatically upload data at the end of each interval, it just simplifies the process of uploading by allowing you to call `log_bundle(&bndl, PUSHINGBOX)` each loop iteration but only having data uploaded at a maximum specified rate

## PushingBox Limitations

PushingBox currently only supports 1000 requests per day per user. This can support a single device sending data at roughly 2 minute intervals. While the various components of PushingBox do not need to be altered for new devices / spreadsheets, another person can log in to PushingBox and essentially create the exact same setup in order to allow an effective additional 1000 requests per day. Note that some of your devices will have to specify this new device ID in order to make use of the additional requests. (See the 'New Script / PushingBox Scenario' section below on how to do this. The part that differs from creating a new script as well, is just skipping to the PushingBox part and creating a service that points to the existing Google Script)

There does not appear to be an easy way to eliminating the PushingBox intermediate as Arduino devices cannot easily make HTTPS get and post requests required to send data directly to the Google script. An HTTPSRedirect library exists but is meant primarily for the ESP8266 Wifi board and it may take a fair amount of work to port to working with the LOOM devices and/or Ethernet. There are alternatives to PushingBox, but they are still intermediates and PushingBox is relatively lightweight.

## Spreadsheet.gs 

This is the Google script that powers the Google Sheets. It turns the data into a dictionary of key-value pairs where the column names are keys, and the data values are values. It stores the current set of columns for each sheet in a user preference cache, allowing the script to know when the number or order of columns change. Data being sent to a sheet must have a key called "tabID" as each unique tabID gets its own sheet/tab in the spreadsheet.

This script is generalized to work with any spreadsheet, so there is no need to create a copy of this script to add sheets or devices. Further, unlike PushingBox, there is not any limit (that we are aware of) to sending data to the script, so duplicates are not necessary even if another user has created a duplicate PushingBox setup. In the event that one wants to modify the script, or have the script be under someone else's account, then that can be done as below:

#### New Script / PushingBox Scenario

- Creating a new Google Sheet

- Selecting Script Editor from the Tools menu

- Copying the spreadsheet.gs file contents into the script editor

- Publish the script by going to Publish > Deploy as Web App

  - In the window that appears, set:
  - 'Project version' to be 'new'
  - 'Execute app as' to be 'Me (email)'
  - 'Who has access to the app' to be 'Anyone, even anonymous'
  - Click update
  - Copy the URL it provides (this is needed by PushingBox)

- Go to PushingBox and create a new service via My Services > Add Service > Custom URL

- Name the service whatever you feel like and copy in Google script URL from above into Root URL, set the Method to GET

- Now you need to create a Scenario that calls this service with data

- Go to My Scenarios and enter a name in the new scenario box and click Add

- The scenario will have a deviceID, make note of this, as this is needed by the Aduino code (in the PushingBox section of the config file)

- Click Add an Action and add the service you just created

- This will bring up a window asking for 'Data' and providing a box to input the Get request parameters

  - Unless you have reason to restrict parameters to be less than 32, this should just be the output of running `python buildstring.py 16`, which is: 

    ```
    ?key0=$key0$&val0=$val0$&key1=$key1$&val1=$val1$&key2=$key2$&val2=$val2$&key3=$key3$&val3=$val3$&key4=$key4$&val4=$val4$&key5=$key5$&val5=$val5$&key6=$key6$&val6=$val6$&key7=$key7$&val7=$val7$&key8=$key8$&val8=$val8$&key9=$key9$&val9=$val9$&key10=$key10$&val10=$val10$&key11=$key11$&val11=$val11$&key12=$key12$&val12=$val12$&key13=$key13$&val13=$val13$&key14=$key14$&val14=$val14$&key15=$key15$&val15=$val15$
    ```

  - ***Note***: The 16 key-value pairs are now not a constraining number, as update code allows for more data. Only the key-value arguments 0-3 should be needed by now, but there is no harm in including the extra pairs (which may allow for legacy support of the old code)

- Now you should be able to call your new script as long as you provide the new deviceID, and a valid spreadsheetID in the config file

### Test Pushingbox

To test PushingBox, build a URL in the format and paste into a browser:

**New Format:**

`http://api.pushingbox.com/pushingbox?devid=[DEVICE-ID]&key0=sheetID&val0=[SHEET-ID]&key2=deviceID&val2=[DEVICE-NAME]&key3=full_data&val3=[datakey1]~[dataval1]~[datakey2]~[dataval2]`

*Individual keys and values are used for the metadata / routing information, while the data itself is contained in a single argument (val3, associated with key3, 'full_data') which is a string of interleaved keys and values separated by tildes (~)*. 

**Old Format:**

`http://api.pushingbox.com/pushingbox?devid=[DEVICE-ID]&key0=sheetID&val0=[SHEET-ID]&key2=deviceID&val2=[DEVICE-NAME]&key3=[KEY-NAME]&val3=[VALUE]`

*This is the old format that was limited to 16 key-value pairs and is associated with the `Legacy_Spreadsheet.gs` Google App script file.*

## Example .ino Loop Contents

This section provides some common examples of how you might format the `loop()` function in the .ino file.

#### WiFi Send and Receive

```
void loop() 
{
	OSCBundle bndl, send_bndl;  		// Bundles to hold incoming and outgoing data

    receive_bundle(&bndl, WIFI);		// Receive messages
    if (bndl.size()) {
        print_bundle(&bndl);			// Print bundle if LOOM_DEBUG enabled
    }
    process_bundle(&bndl);				// Dispatch message to correct handling functions
    measure_sensors();					// Read sensors, store data in sensor structs
    package_data(&send_bndl);			// Copy sensor data from state to provided bundle
    send_bundle(&send_bndl, WIFI);		// Send bundle of packaged data						additional_loop_checks();			// Miscellaneous checks
}
```

#### LoRa Hub (Recieve from node, upload to Google Sheet, no sensors)

```
void loop() 
{
	OSCBundle bndl;  					// Bundles to hold incoming and outgoing data

	receive_bundle(&bndl, LORA);		// Receive messages
	log_bundle(&bndl, PUSHINGBOX);		// Send bundle of packaged data
	additional_loop_checks();			// Miscellaneous checks
}
```

#### LoRa Sensor Node (Transmits to central hub for upload to Google Sheet)

```
void loop() 
{
	OSCBundle send_bndl;  				// Bundles to hold incoming and outgoing data

	measure_sensors();					// Read sensors, store data in sensor structs
	package_data(&send_bndl);			// Copy sensor data from state to provided bundle
	send_bundle(&send_bndl, LORA);		// Send bundle of packaged data
	additional_loop_checks();			// Miscellaneous checks
}
```



#### 

#### 