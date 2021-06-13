
# Generate configuration options for Loom devices
# The exported json/yaml can then be parsed by some config-generating UI
# or just used for reference
# Depends on code adhering to documentation format describe at https://github.com/OPEnSLab-OSU/Loomify
#
# Module parameter documenation takes the form:
# Doxygen comment   Name....   Type   Default   Range/Set....   Description..............................
# /// @param[in]    enableA0   Bool | <false> | {true, false} | Whether or not to use pin A0 for Neopixel
#
# Usage:
# python3 get_configuration_options.py /path/to/Loom

# TODO: Update to indicate what factory enable settings are required for each module

import subprocess
import json
import yaml
import sys
import os


ENABLE_SET_LABELS = False	# If valid options are a set with labels, 
							# ENABLE_SET_LABELS controls whether labels are output 
							# (each item become list [value, label])


def processSetItem(setItem, optionType, useSetLabels=False):
	item, label = ((tmp := setItem.partition('('))[0], tmp[2].strip(' ")')) if '(' in setItem else (setItem, None)
	item = item.strip(' <>"')

	try:
		if item.isnumeric(): # Don't check optionType as Int, as it might be an enum name 
			item = int(item)
		elif 'bool'  in optionType.lower() and item.lower() == 'true': 
			item = True
		elif 'bool'  in optionType.lower() and (item.lower() == 'false'): 
			item = False
		elif 'float' in optionType.lower():
			item = float(item)
		elif item.startswith('0x'):
			tmp = int(item, 16)
			if label == None:
				label = item
			item = tmp
	except:
		pass

	if useSetLabels:
		return [item, label] if label != None else item
	else:
		return item



def processValidOptions(options, optionType):

	# Set
	if options.startswith('{'):
		return list(map(lambda x: processSetItem(x, optionType, ENABLE_SET_LABELS), options.strip('{}').split(',')))

	# Range
	if options.startswith('['):
		start, _, end = options.strip('[]').partition('-')
		try:
			if optionType == 'Int':
				return [int(start), int(end)]
			if optionType == 'Float':
				return [float(start), float(end)]
		except:
			return []



def parseOptionsList(options):
	try:
		return [
			{ 
				'parameter':       (name := option.split()[1]),
				'details': {
					'type':        (desc := list(map(lambda x: x.strip(), 
										option.partition(name)[2].split('|'))) )[0],

					'default':      processSetItem(desc[1], desc[0]),

					'set' if ('set' in desc[0].lower() or 'bool' in desc[0].lower()) 
					else 'range':       
									processValidOptions(desc[2], desc[0]),

					'description': desc[3]
				}
			}
			for option in options
		]
	except:
		return []



def getGeneralSettings(path):

	filename = os.path.join(path, 'src/Manager.h')

	### Get doxygen documentation and parameters of primary module constructor
	documentation, _, parameters = subprocess.run(
		f"pcregrep -M 'CONSTRUCTORS.*(.|\n)*?\);' {filename}",
		shell=True, capture_output=True) \
		.stdout.decode("utf-8") \
		.partition(f"Manager(")

	options = [line.replace('\t', ' ').replace('///', '').strip() for line in documentation.split('\n') if '@param' in line]

	### Get parameter info from code, not currently used (relying on documentation comments to be accurate)
	# parameters = [cleaned for line in parameters.split('\n') 
					# if len(cleaned := line.replace('\t', ' ').rstrip(',);').strip()) > 0]

	return parseOptionsList(options)



def getModuleSettings(path):

	### Find files that contain module declarations
	# Module that have settings will be those that contain 'REGISTER(Module' in them
	REGISTERED_MODULES = "REGISTER(Module"
	moduleFiles = filter(
		lambda f: f != '', 
		subprocess.run(f"grep {path} --include=*.h -rle '{REGISTERED_MODULES}'", 
		shell=True, capture_output=True)
		.stdout.decode("utf-8").split('\n'))


	moduleSettings = []

	for filename in moduleFiles:

		### Get module name and super class
		try:
			_, module, _, _, superClass = subprocess.run(f"grep {filename} -e 'class.*: public'", 
				shell=True, capture_output=True).stdout.decode("utf-8").strip().split(' ')
		except:
			# Improper formatting, skipping
			continue

		moduleComment = subprocess.run(
			f"pcregrep -M 'namespace(.|\n)*class {module} ?:' {filename} | pcregrep -M '///+\n///(.|\n)*///'",
			shell=True, capture_output=True).stdout.decode("utf-8")
		desc, _, resources = moduleComment.partition('@par Resources')

		### Build module info dict
		moduleInfo = {
			'category': superClass,
			'description': '\n'.join([ cleaned for line in desc.split('\n') if (cleaned := line.lstrip(' /')) ]),
			'resources': {
				res[0]: res[2].partition(' ')[0].strip(')')                 # Link label : hyperlink (with some clean up of edge cases with text after link)
				for line in resources.split('\n')
				if (res := line.strip('/ \t-[()').partition(']('))[0] != '' # Split into link label and hyperlink
			}
		}
		moduleInfo = {'module': module } | moduleInfo # this added after just so 'module' is first key (keys in objects don't have a defined order)


		### Get doxygen documentation and parameters of primary module constructor
		documentation, _, parameters = subprocess.run(
			f"pcregrep -M 'CONSTRUCTORS.*(.|\n)*?\);' {filename}",
			shell=True, capture_output=True) \
			.stdout.decode("utf-8") \
			.partition(f"{module}(")

		options = [line.replace('\t', ' ').replace('///', '').strip() for line in documentation.split('\n') if '@param' in line]
		
		### Get parameter info from code, not currently used (relying on documentation comments to be accurate)
		# parameters = [cleaned for line in parameters.split('\n') 
						# if len(cleaned := line.replace('\t', ' ').rstrip(',);').strip()) > 0]

		### Build module info/config dictionary
		moduleInfo['settings'] = parseOptionsList(options)
		moduleSettings.append(moduleInfo)

	return moduleSettings



if __name__ == "__main__":

	path = os.path.abspath(sys.argv[1]) if len(sys.argv) > 2 else '..'

	configurationOptions = {
		'general':    getGeneralSettings(path),
		'components': getModuleSettings(path)
	}

	# Print configuration options
	# print(json.dumps(configurationOptions, sort_keys=False, indent=2))
	print(yaml.dump(configurationOptions, indent=2, sort_keys=False))

	# Write to files
	with open('configOptions.json', 'w') as file:
		json.dump(configurationOptions, file, indent=4)

	with open('configOptions.yaml', 'w') as file:
		yaml.dump(configurationOptions, file, indent=2, sort_keys=False)







