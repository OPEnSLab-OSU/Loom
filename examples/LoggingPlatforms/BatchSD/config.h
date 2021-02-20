"{\
	'general':\
	{\
		'name':'Device',\
		'instance':1,\
		'interval':5000\
	},\
	'components':[\
		{\
			'name':'Analog',\
			'params':'default'\
		},\
		{\
			'name':'SD',\
			'params':[true,1000,10,'data',true]\
		},\
    		{\
      			'name':'BatchSD',\
      			'params': [true, 1000, 10]\
    		},\
		{\
			'name':'LTE',\
			'params':['<apn>','<username>','<password>', '<analog_pin>']\
		},\
	    	{\
	      		'name':'GoogleSheets',\
	      		'params':['Goog', 7003,'/macros/s/<your-script-id>/exec',\
	        '<your-sheet-id>',\
	/*true to autoname tab*/				true,\
	/*not used if previous param is true*/	'<your-tab-name>'\
	      			]\
	    	}\
/*    		{\
      			'name':'nRF',\
      			'params':[120,01,1,0,3,200,1]\
    		},\
    		{\
			'name':'LoRa',\
			'params':[255, 0, 23, 3, 200]\
		},\
    		{\
			'name':'Bluetooth',\
			'params':'default'\
		}\
	*/]\
}"
