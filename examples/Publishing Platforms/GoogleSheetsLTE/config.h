"{\
	'general':\
	{\
		'device_name':'Device',\
		'instance_num':1,\
		'interval':3000\
	},\
	'components':[\
		{\
			'name':'Analog',\
			'params':'default'\
		},\
		{\
			'name':'Digital',\
			'params':'default'\
		},\
		{\
			'name':'LTE',\
			'params':['<apn>','<username>','<password>', '<analog_pin>'\
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
