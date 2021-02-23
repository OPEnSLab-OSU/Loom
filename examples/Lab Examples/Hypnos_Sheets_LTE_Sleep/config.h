"{\
	'general':\
	{\
		'name':'Device',\
		'instance':1,\
		'interval':5000,\
		'print_verbosity':2\
	},\
	'components':[\
		{\
			'name':'SD',\
			'params': 'default'\
		},\
		{\
			'name':'DS3231',\
			'params':'default'\
		},\
		{\
			'name':'Interrupt_Manager',\
			'params':[0]\
		},\
		{\
			'name':'Sleep_Manager',\
			'params':[true,false,1]\
		},\
    {\
			'name':'LTE',\
			'params':['<apn>','<username>','<password>', '<analog_pin>']\
		},\
		{\
			'name':'GoogleSheets',\
			'params':[\
				'Goog',\
				7003,\
				'/macros/s/<your-script-id>/exec',\
				'<your-sheet-id>',\
/*true to autoname tab*/				true,\
/*not used if previous param is true*/	'<your-tab-name>'\
			]\
		}\
	]\
}"
