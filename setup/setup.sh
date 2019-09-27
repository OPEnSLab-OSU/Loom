#!/bin/sh -e


# Download Arduino CLI to assist in installation
download_arduino_cli()
{
	curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | BINDIR=/usr/local/bin sh
}

# Setup Arduino cores and board profiles
arduino_setup()
{
	# Update core index
	arduino-cli core update-index

	# Install Arduino SAMD board profiles
	arduino-cli core install arduino:samd

	# Install Adafruit SAMD board profiles
	arduino-cli core install adafruit:samd --additional-urls https://adafruit.github.io/arduino-board-index/package_adafruit_index.json

	# Create Arduino folder
	mkdir -p ~/Documents/Arduino/libraries
}

# Download regular Loom dependencies
download_loom_dependencies()
{
	# Install Loom dependencies (from library manager
	curl -fsSL https://raw.githubusercontent.com/OPEnSLab-OSU/Loom/master/setup/library_list.txt | xargs -n1 arduino-cli lib install
}

# Download Loom dependencies not in library manager
download_extra_loom_dependencies()
{
	# Install Loom dependencies (not in library manager)
	curl -fsSL https://raw.githubusercontent.com/OPEnSLab-OSU/Loom/master/setup/library_list2.txt | xargs -n1 -I % sh -c '{ curl -sL % | tar xC ~/Documents/Arduino/libraries/; sleep 1; }'
}

# Install Loom and its dependencies
install_loom()
{
	# Get Loom and install Loom
	# curl -sL https://github.com/OPEnSLab-OSU/Loom/archive/1.0.0.zip | tar xC ~/Documents/Arduino/libraries/
	curl -sL https://github.com/OPEnSLab-OSU/Loom/archive/master.zip | tar xC ~/Documents/Arduino/libraries/

	# Install dependencies
	download_loom_dependencies
	download_extra_loom_dependencies
}

# Open link to download IDE
open_ide_link()
{
	# Make this optional based on if already installed
	open https://www.arduino.cc/en/main/software

	echo "A link should have opened to download the Arduino IDE"
	echo "If it did not, open it here: https://www.arduino.cc/en/main/software"
}

# Run any post install cleanup
cleanup()
{
	# Remove possible __MACOSX folder
	rm -R ~/Documents/Arduino/libraries/__MACOSX
}

# Pre-exit behavior if signal caught
terminate()
{
	echo "Caught Signal ... cleaning up."
	
	cleanup

	echo "Done cleanup ... quitting."
	exit 1
}


# # # # # # # # # # # # # # # # # # # # # # # #

# Trap signals to exit gracefully
trap terminate 1 2 3 6
download_arduino_cli
arduino_setup
install_loom
open_ide_link
cleanup


