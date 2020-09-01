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
	arduino-cli core update-index --additional-urls https://adafruit.github.io/arduino-board-index/package_adafruit_index.json,https://github.com/OPEnSLab-OSU/Loom_Auxiliary/raw/master/package_loom_index.json

	# Install Arduino SAMD board profiles
	arduino-cli core install arduino:samd

	# Install Adafruit SAMD board profiles
	arduino-cli core install adafruit:samd@1.5.7 --additional-urls https://adafruit.github.io/arduino-board-index/package_adafruit_index.json
	arduino-cli core install loom:samd --additional-urls https://github.com/OPEnSLab-OSU/Loom_Auxiliary/raw/master/package_loom_index.json

	echo "boardsmanager.additional.urls=https://adafruit.github.io/arduino-board-index/package_adafruit_index.json,https://github.com/OPEnSLab-OSU/Loom_Auxiliary/raw/master/package_loom_index.json" >> Library/Arduino15/preferences.txt

	# Create Arduino folder
	mkdir -p ~/Documents/Arduino/libraries
}


# Open link to download IDE
open_ide_link()
{
	# Make this optional based on if already installed
	open https://www.arduino.cc/en/main/software

	echo "A link should have opened to download the Arduino IDE"
	echo "If it did not, open it here: https://www.arduino.cc/en/main/software"
}


# Pre-exit behavior if signal caught
terminate()
{
	echo "Caught Signal ... quitting"

	exit 1
}


# # # # # # # # # # # # # # # # # # # # # # # #

# Trap signals to exit gracefully
trap terminate 1 2 3 6
download_arduino_cli
arduino_setup
open_ide_link
