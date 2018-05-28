# importing the requests library
from collections import OrderedDict
import requests
import sys
 
def main():
	if len(sys.argv) == 2: 
		num_cols = int(sys.argv[1])
		id_tag_v = raw_input("IDtag: ")
		devid_v = str(raw_input("PushingBox Device ID: "))
	elif len(sys.argv) == 3: 
		num_cols = int(sys.argv[1])
		id_tag_v = sys.argv[2]
		devid_v = raw_input("PushingBox Device ID: ")
	elif len(sys.argv) == 4:
		num_cols = int(sys.argv[1])
		id_tag_v = sys.argv[2]
		devid_v = sys.argv[3]
	else:
		num_cols = int(raw_input("Number of columns: "))
		id_tag_v = raw_input("IDtag: ")
		devid_v = str(raw_input("PushingBox Device ID: "))

	keyList = ['IDtag'] +  ['tmpKey'+str(i) for i in xrange(num_cols-1)]
	valList = [str(id_tag_v)] + ['tmpVal'+str(i) for i in xrange(num_cols-1)]

	param_dict = OrderedDict()
	param_dict['devid'] = str(devid_v)
	for i in range(len(keyList)):
			param_dict['key' + str(i)] = keyList[i]
			param_dict['val' + str(i)] = valList[i]

	URL = "http://api.pushingbox.com/pushingbox?"
	 
	# sending get request and saving the response as response object
	r = requests.get(url = URL, params = param_dict)
	print "Get request sent!\n"

if __name__ == "__main__":
	main()

