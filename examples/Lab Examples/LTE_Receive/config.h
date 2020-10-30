"{\
	'general':\
	{\
		'name':'Device',\
		'instance':5,\
		'interval':5000,\
		'print_verbosity':2\
	},\
	'components':[\
    {\
      'name':'LoRa',\
      'params':[255,1,23,3,200]\
    },\
    {\
      'name':'LTE',\
      'params':['<apn>','<username>','<password>', '<analog_pin>']\
    },\
    {\
      'name':'GoogleSheets',\
      'params':[\
        'Google',\
        7003,\
        '/macros/s/<your-script-id>/exec',\
				'<your-sheet-id>',\
/*true to autoname tab*/				true,\
/*not used if previous param is true*/	'<your-tab-name>'\
      ]\
      }\
	]\
}"
