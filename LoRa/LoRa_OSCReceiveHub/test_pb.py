# importing the requests library
from collections import OrderedDict
import requests
 
# api-endpoint
devid_v = "v25CCAAB0F709665"
id_tag_v = "/LOOM/DShield2/Decagon1"
timestamp_v = "2018"
temp_c_v = "4C"
humid_v = "33"
load_cell_v = "1000"
ir_light_v = "2000"
full_light_v = "3000"
bat_volt_v = "4.2"
temp_field0_v = "hello there"
temp_field1_v = "general kenobi"
temp_field2_v = "i am the senate"
temp_field3_v = "do it"
temp_field4_v = "hello there"
temp_field5_v = "general kenobi"
temp_field6_v = "i am the senate"

keyList = ['IDtag', 'TimeStamp', 'TempC', 'Humid', 'loadcell', 'irlight', 'fullight',
           'batvolt', 'temp0', 'temp1', 'temp2', 'temp3', 'temp4', 'temp5', 'temp6']
valList = [id_tag_v, timestamp_v, temp_c_v, humid_v, load_cell_v, ir_light_v,
           full_light_v, bat_volt_v, temp_field0_v, temp_field1_v, temp_field2_v, temp_field3_v, temp_field4_v, temp_field5_v, temp_field6_v ]

param_dict = OrderedDict()
param_dict['devid'] = devid_v
for i in range(len(keyList)):
    param_dict['key' + str(i)] = keyList[i]
    param_dict['val' + str(i)] = valList[i]

URL = "http://api.pushingbox.com/pushingbox?"
 
# sending get request and saving the response as response object
r = requests.get(url = URL, params = param_dict)
print r.url


