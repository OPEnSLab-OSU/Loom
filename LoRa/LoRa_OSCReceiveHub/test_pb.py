# importing the requests library
from collections import OrderedDict
import requests
 
# api-endpoint
devid_v = "v25CCAAB0F709665"
id_tag_v = "3"
timestamp_v = "2018"
temp_c_v = "4C"
humid_v = "33"
load_cell_v = "1000"
ir_light_v = "2000"
full_light_v = "3000"
bat_volt_v = "4.2"

keyList = ['devid', 'IDtag', 'TimeStamp', 'TempC', 'Humid', 'loadcell', 'irlight', 'fullight',
           'batvolt']
valList = [devid_v, id_tag_v, timestamp_v, temp_c_v, humid_v, load_cell_v, ir_light_v,
           full_light_v, bat_volt_v]

param_dict = OrderedDict()
for i in range(len(keyList)):
    param_dict['key' + str(i)] = keyList[i]
    param_dict['val' + str(i)] = valList[i]

payload = (('devid', devid_v), ('IDtag', id_tag_v), ('TimeStamp', timestamp_v),
           ('TempC', temp_c_v), ('Humid', humid_v), ('LoadCell', load_cell_v),
           ('IRLight', ir_light_v), ('FullLight', full_light_v), ('BatVolt', bat_volt_v))

URL = "http://api.pushingbox.com/pushingbox?" 
 
# sending get request and saving the response as response object
r = requests.get(url = URL, params = param_dict)
print r.url


