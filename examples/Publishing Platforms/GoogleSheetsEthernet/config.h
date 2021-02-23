"{\
	'general':\
	{\
		'name':'Device',\
		'instance':1,\
		'interval':10000\
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
			'name':'Ethernet',\
			'params':[\
				'Ether1',\
				['<your-mac-address>'],\
				[192,168,0,1]\
			]\
		},\
		{\
			'name':'GoogleSheets',\
			'params':[\
				'Goog',\
				7001,\
				'/macros/s/<your-script-id>/exec',\
				'<your-sheet-id>',\
/*true to autoname tab*/				true,\
/*not used if previous param is true*/	'<your-tab-name>'\
			]\
		}\
	]\
}"