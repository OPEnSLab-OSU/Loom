"{\
	'general':\
	{\
		'name':'Device',\
		'instance':1\
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
				[<your-mac-address>],\
				[192,168,0,1]\
			]\
		},\
		{\
			'name':'DS3231',\
			'params':[11,true,true,6]\
		},\
		{\
			'name':'NTP_Sync',\
			'params':['NTP1',0,0]\
		},\
		{\
			'name':'GoogleSheets',\
			'params':[\
				'Goog',\
				0,\
				'/macros/s/<your-script-id>/exec',\
				'<your-sheet-id>',\
				'<your-tab-name>',\
				'<your-device-name>'\
			]\
		}\
	]\
}"