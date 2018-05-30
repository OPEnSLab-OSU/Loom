/*This needs to be applied using the script editor tool within google spreadsheets in order to function. 


Specifics on how to set this up can be found here. https://www.hackster.io/detox/transmit-esp8266-data-to-google-sheets-8fc617


Code:*/
//-----------------------------------------------
//Originally published by Mogsdad@Stackoverflow
//Modified for The OPEnS Lab
//Modified by Thomas DeBell
//-----------------------------------------------
/*

/* Using spreadsheet API */

//Run this in order to test the doGet(e) function
function fakeGet() {
  var eventObject = 
    {
      "parameter": {
        "key0": "devid",
        "val1": "1",
        "key3": "Humid",
        "val0": "asd23423v",
        "val3": "4%",
        "val2": "12C",
        "key1": "IDtag",
        "key2": "Temp"
        
      },
      "contextPath": "",
      "contentLength": -1,
      "queryString": "action=view&page=3",
      "parameters": {
        "key0": ["devid"],
        "key1": ["IDtag"],
        "val0": ["asd23423v"],
        "val1": ["1"],
        "key2": ["Temp"],
        "val2": ["null"],
        "key3": ["Humid"],
        "val3": ["null"]
      }
    }
  doGet(eventObject);
}

// Automatically called whenever the linked PushingBox Scenario
// has data sent to it in the form of key-value pairs. 
function doGet(e) { 
  var result = 'Ok'; // assume success

  if (e.parameter == undefined) {
    result = 'No Parameters';
  }
  else {
    //specific to the spreadsheet you are working on. this is id is found     
    //directly after "docs.google.com/spreadsheetURL/d" in the URL. 
    var id = '1kkD8Tql4uJ7usnIM5TLM35XSYinU2_JmILoUw8eEjg4';
    
    //Turn the Get Request arguments into a dictionary using the key-value pairs from the request.
    var data_dict = getDataDict(e.parameter);
    
    //The keys from data_dict is the set of columns in the spreadsheet.
    var column_set = Object.keys(data_dict);
    //The name of the sheet for the data to go to is the IDtag field that is sent.
    var sheet_id = stripQuotes(data_dict['IDtag']);
    var spreadsheet = SpreadsheetApp.openById(id);
    var sheet_list = spreadsheet.getSheets();
    //Work on the sheet that the data belongs to
    var sheet = spreadsheet.getSheetByName(sheet_id);
    
    //If the IDtag is new, then create a sheet for it.
    if(sheet == undefined) {
      sheet = spreadsheet.insertSheet(sheet_id);
      sheet_list = spreadsheet.getSheets();
      sheet.appendRow(column_set);
      setSheetColumnList(sheet.getName(), column_set);
    }
    
    var new_range;
    var new_row = sheet.getLastRow() + 1;
    var row_data = [];
    
    //If the set of data received is different than previous for the sheet
    //Put a break between the two data sets and label the columns accurately.
    if(!arraysEqual(column_set, getSheetColumnList(sheet.getName()))) {
      new_row += 1;
      new_range = sheet.getRange(new_row, 1, 1, column_set.length);
      new_range.setValues([column_set]);
      new_row += 1;
      setSheetColumnList(sheet.getName(), column_set);
    }
   
    //The data from the rows are the values from the dictionary where the key is the column name.
    for(i = 0; i < column_set.length; i++)
      row_data.push(data_dict[column_set[i]]);
    
    //Write the row to the sheet.
    new_range = sheet.getRange(new_row, 1, 1, row_data.length);
    new_range.setValues([row_data]); 
  }

  // Return result of operation
  return ContentService.createTextOutput(result);
}

//Takes in the arguments of the get request
//Formats it into a dictionary linking keys to their values.
function getDataDict(get_args) {
  var data_dict = {};
  var col_list = getMatches(get_args, "key[0-9]+");
  var val_list = getMatches(get_args, "val[0-9]+");
  for(i = 0; i < col_list.length; i++)
    if(val_list[i] != "null")
      data_dict[col_list[i]] = val_list[i];
  return data_dict;
}

//Returns the subset of args that matched pattern patt
//as a list.
function getMatches(args, patt) {
  var list = [];
  var match_order = [];
  var key_list = Object.keys(args);
  var match_patt = new RegExp(patt);
  
  for(var key in key_list)
    if(match_patt.test(key_list[key]))
      match_order.push(key_list[key]);
  match_order.sort();

  for (var match in match_order)
      list.push(args[match_order[match]]);
  return list;
}

//Returns a string of columns delimited by ','
function getColumnString(col_list) {
  var col_string = '';
  for (var i in col_list)
    col_string += (col_list[i] + ',')
  return col_string.slice(0, col_string.length - 1);
}

//Returns a list of column names
function getColumnList(col_string) {
  return col_string.split(',');
}

//Sets the set of columns that a sheet is using
function setSheetColumnList(sheet_name, col_list) {
  var doc_properties = PropertiesService.getDocumentProperties();
  doc_properties.setProperty(sheet_name, getColumnString(col_list));
}

//Gets the set of columns that a sheet is using
function getSheetColumnList(sheet_name) {
  var doc_properties = PropertiesService.getDocumentProperties();
  return getColumnList(doc_properties.getProperty(sheet_name));
}

/**
* Remove leading and trailing single or double quotes
*/
function stripQuotes( value ) {
  return value.replace(/^["']|['"]$/g, "");
}

//Checks if two arrays are equal to each other.
function arraysEqual(a, b) {
  if (a === b) return true;
  if (a == null || b == null) return false;
  if (a.length != b.length) return false;
  
  for (var i = 0; i < a.length; ++i) {
    if (a[i] !== b[i]) return false;
  }
  return true;
}