import click
import serial
from serial.tools.list_ports import comports
import parse
import enum
import subprocess
import threading
import time
import json
from colorama import init, Fore, Back

init(autoreset=True)

FQBN = "adafruit:samd:adafruit_feather_m0"
PID_SKETCH = 32779
PID_BOOTLOADER = 11
VID = 9114
BAUD = 115200

STATUS = b'R\r\n'
DIAGNOSE = b'D\r\n'
FLASH_MODE = b'M\r\n'
START_LOOM = b'S\r\n'

# F:OK
FLASH_RES = b'F:OK\r\n'
# M:OK
FLASH_MODE_RES = b'M:OK\r\n'
# R:(Device name),(Device in bootloader mode, 0 or 1),(Last Build Time ctime),(Last Bootloader Time ctime)\r\n
STATUS_RES = parse.compile('R:"{device_name:l}",{mode_boot:d=1},"{build_time:tc}","{boot_time:tc}"\r\n')
# D:(Sensor name),(success, 0 or 1),(string error)\r\n
DIAGNOSE_RES = parse.compile('D:"{sensor_name:l}",{success:d=1},"{error:l}"\r\n')
# S:OK
START_RES = b'S:OK\r\n'

class Status(enum.Enum):
    OK = 0
    BOOTLOADER = 1
    NOT_FLASHED = 2
    CONFIG_READY = 3
    HUNG = 4

class Type(enum.Enum):
    ANY = 0
    BOOTLOADER_ONLY = 1
    SKETCH_ONLY = 2

class Level(enum.Enum):
    OK = Back.GREEN + Fore.BLACK
    INFO = Back.RESET + Fore.RESET
    WARN = Back.YELLOW + Fore.BLACK
    ERROR = Back.RED + Fore.BLACK

# functions to find and check serial ports
def build_sketch(cli, bin_path, sketch):
    """./arduino-cli/arduino-cli.exe compile -b adafruit:samd:adafruit_feather_m0 -o loom.bin ../../Base"""
    try:
        cli.compile(sketch, fqbn=FQBN, output=bin_path)
    except pyduinocli.errors.arduinoerror.ArduinoError as e:
        return e
    return True

def f(printed, level):
    return str(level.value) + printed + str(Level.INFO.value)

CLICK_MUTEX = threading.Lock()
def print_serial_status(level, address, msg):
    with CLICK_MUTEX:
        click.echo(f'{ f(address, level) }: {msg}')
        time.sleep(0.25)

def get_serial_ports(exclude):
    boards_all = comports()
    # get the COM port, if the PID matches the sketch or bootloader and the VID is adafruit
    return [ (board.device, board.pid) for board in boards_all 
        if board.device not in exclude
        and board.vid == VID
        and (board.pid == PID_SKETCH or board.pid == PID_BOOTLOADER)]

def poll_serial_port(address, pid):
    try:
        if pid == PID_BOOTLOADER:
            print_serial_status(Level.INFO, address, 'Bootloader mode')
            return Status.BOOTLOADER
        # else poll the device
        elif pid == PID_SKETCH:
            with serial.Serial(address, BAUD, timeout=2, write_timeout=2, inter_byte_timeout=2) as serial_port:
                # poll the device
                serial_port.write(STATUS)
                status = None
                # if the read times out, the device probably just isn't flashed
                try:
                    status = serial_port.readline().decode()
                except serial.SerialTimeoutException:
                    print_serial_status(Level.WARN, address, 'No status from port')
                    return Status.NOT_FLASHED
                # else if we get an invalid status, same reason
                result = STATUS_RES.parse(status)
                if result is None:
                    print_serial_status(Level.WARN, address, f'Invalid status: "{ status.rstrip() }"')
                    return Status.NOT_FLASHED
                # else the device is flashed! check the status
                print_serial_status(Level.INFO, address, 'Mode: '
                    + ("Bootloader" if result["mode_boot"] else "Sketch")
                    + f', Built: { result["build_time"] }, Boot: { result["boot_time"] }')
                if result["mode_boot"]:
                    return Status.CONFIG_READY
                else:
                    return Status.OK
        else:
            print_serial_status(Level.ERROR, address, f'Unknown PID {pid}')
            return Status.HUNG
    except serial.SerialTimeoutException:
        # uh oh, looks like the serial port didn't open
        print_serial_status(Level.WARN, address, f'Hung, or not compiled with Serial.')
        return Status.HUNG

def send_serial_command(address, cmd, response):
    with serial.Serial(address, BAUD, timeout=2, write_timeout=2, inter_byte_timeout=2) as serial_port:
        try:
            serial_port.write(cmd)
            status = serial_port.readline()
            if status != response:
                print_serial_status(Level.ERROR, address, f'Recieved invalid status {status.decode()} when sensing command {cmd.decode().rstrip()}')
                return False
        except serial.SerialTimeoutException:
            print_serial_status(Level.ERROR, address, f'Timeout when sending command {cmd.decode()}')
            return False
    return True

def reset_board_bootloader(address):
    # stolen from http://markparuzel.com/?p=230
    ser = serial.Serial(timeout=2, inter_byte_timeout=2)
    ser.port = address
    ser.open()
    ser.baudrate = 1200 # This is magic.
    ser.rts = True
    ser.dtr = False
    ser.close()

def upload_serial_port(bossac_path, bin_path, address):
    result = subprocess.run([bossac_path, f'--port={address}', '--force_usb_port=true', '-e', '-w', '-v', '-R', bin_path], capture_output=True)
    if result.returncode != 0:
        print_serial_status(Level.ERROR, address, f'Upload failed with error: {result.stderr}')
        return False
    print_serial_status(Level.OK, address, 'Upload succeded')
    return True

def flash_config(address, sub_json, config):
    # echo!
    print_serial_status(Level.INFO, address, f'Sending values "{json.dumps(sub_json)}"')
    # send all these key/value pairs to the device
    send_bytes = bytes(f'F:\'{config}\',' + ','.join([f'\'{key}\':\'{value}\'' for key,value in sub_json.items()]) + '\r\n', 'utf8')
    return send_serial_command(address, send_bytes, FLASH_RES)

             