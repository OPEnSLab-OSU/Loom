# PushingBox to Google Sheet Pipeline

## Data Pipeline - 
The LoRa hub receives data and makes a GET request to a PushingBox Scenario using the data it's trying to send as URL arguments. The PushingBox Scenario then formats this data and uses the formatted data as URL arguments for the GET request it makes of the Google Sheet, which then parses and displays the data.

## Spreadsheet.gs - 
This is the Google script that powers the Google Sheet. It turns the data into a dictionary of key-value pairs where the column names are keys, and the data values are values. It stores the current set of columns for each sheet in a user preference cache, allowing the script to know when the number or order of columns change. Data being sent to a sheet must have a key called "IDtag" as each unique IDtag gets its own sheet/tab in the spreadsheet.

## buildstring.py - 
This is a python script that will build the string used by the PushingBox Scenario. The usage is either "python buildstring.py", in which case the user will be prompted to enter the number of key-value pairs / columns. Or "python buildstring.py num_cols" where num_cols is the integer number of columns/key-value pairs of data being sent. The output is always a string to the command-line, which can be copied and pasted into the PushinBox Scenario.

## fake_get.py -
This is the python script used to test the PushingBox and Google Script functionality without needing to set up a LoRa Hub. It can be run in 3 ways:

- "python fake_get.py <num_cols>" - Where <num_cols> is the number of columns/key-value pairs being sent (including IDtag).
	
- "python fake_get.py <num_cols> <IDtag>" - Where <IDtag> is the desired name of the sheet/tab.
	
- "python fake_get.py <num_cols> <IDtag> <Device ID>" - Where <Device ID> is the unique identifier of the PushingBox Scenario. 
	
The script will query the user for the information that is not supplied.