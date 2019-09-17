"{\
	'general':\
	{\
		'name':'Ishield',\
		'instance':1,\
		'interval':50\
	},\
	'components':[\
		{\
			'name':'Analog',\
			'params':[8,12,true,true,true,false,false,false,0,0,0,0,0,0,25.0]\
		},\
		{\
			'name':'Digital',\
			'params':'default'\
		},\
		{\
			'name':'WiFi',\
			'params':[\
				'<network name>',\
				'<password>'\
			]\
		},\
		{\
			'name':'MaxPub',\
			'params':[7002]\
		},\
		{\
			'name':'MaxSub',\
			'params':[7002,true]\
		},\
		{\
			'name':'MPU6050',\
			'params':[104,255,0]\
		}\
	]\
}"