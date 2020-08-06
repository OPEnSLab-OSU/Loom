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
			'name':'Spool',\
			'params':[\
				                                                                                'Spool1',\
				                                                                                7003,\
				                                                                                'device.open-sensing.org',\
				                                                                                '/api/device/data',\
/*same as device-id*/                                                                           '<coordinator-id>',/
                                                                                                '<device-id>',/
                                                                                                '101',/
/*must include '-----BEGIN CERTIFICATE-----\n' and '\n-----END CERTIFICATE-----\n' part*/       '<certificate>',\
/*must include '-----BEGIN EC PRIVATE KEY-----\n' and '\n-----END EC PRIVATE KEY-----\n' part*/ '<private-key>'\
			]\
		}\
	]\
}"