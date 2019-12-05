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
import pyduinocli

init(autoreset=True)

FQBN = "adafruit:samd:adafruit_feather_m0"
PID_SKETCH = 32779
PID_BOOTLOADER = 11
VID = 9114
BAUD = 9600

STATUS = b'R\r\n'
DIAGNOSE = b'D\r\n'
FLASH_MODE = b'M\r\n'
START_LOOM = b'S\r\n'
WIPE_SD = b'W\r\n'

CFG_NAME = "cfg.json"

# F:OK
FLASH_RES = b'F:OK\r\n'
# M:OK
FLASH_MODE_RES = b'M:OK\r\n'
# R:(Device name),(Device has been flashed, 0 or 1),(Last Build Time ctime),(Last Bootloader Time ctime)\r\n
STATUS_RES = parse.compile('R:"{device_name:w}",{is_flashed:d=1},"{build_time:tc}","{boot_time:tc}"\r\n')
# D:(Sensor name),(success, 0 or 1),(string error)\r\n
DIAGNOSE_RES = parse.compile('D:"{sensor_name:l}",{success:d=1},"{error:l}"\r\n')
# S:OK
START_RES = b'S:OK\r\n'
# C:OK
WRITE_FILE_RES = b'C:OK\r\n'
# W:OK
WIPE_SD_RES = b'W:OK\r\n'
# error
ERROR_RES = b'ERROR'

class Status(enum.Enum):
    OK = 0
    BOOTLOADER = 1
    NOT_FLASHED = 2
    HUNG = 3
    ERROR = 4

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
    if pid == PID_BOOTLOADER:
        print_serial_status(Level.INFO, address, 'Bootloader mode')
        return Status.BOOTLOADER
    if pid != PID_SKETCH:
        print_serial_status(Level.ERROR, address, f'Unknown PID {pid}')
        return Status.HUNG
    
    result = send_serial_command(address, STATUS, STATUS_RES)
    if isinstance(result, parse.Result):
        print_serial_status(Level.INFO, address, f'Instance: { result["device_name"] }, '
            + f'Flashed: { "Yes" if int(result["is_flashed"]) == 1 else "No" }, '
            + f'Built: { result["build_time"] }, Flashed: { result["boot_time"] }')
        return Status.OK
    else:
        print_serial_status(Level.WARN, address, f'Status {result}')
        return result

def send_serial_command(address, cmd, response, timeout=2):
    try:
        # else poll the device
        with serial.Serial(address, BAUD, timeout=timeout, write_timeout=timeout, inter_byte_timeout=timeout) as serial_port:
            # poll the device
            result_decoded = None
            status = None
            attempt = 0
            while attempt < 5 and result_decoded is None and (status is None or ERROR_RES not in status):
                # flush the serial buffer
                serial_port.reset_input_buffer()
                # write the status command
                serial_port.write(cmd)
                serial_port.flush()
                while True:
                    # if the read times out, the device probably just isn't flashed
                    try:
                        status = serial_port.readline()
                    except serial.SerialTimeoutException:
                        print_serial_status(Level.WARN, address, 'No status from port')
                        return Status.NOT_FLASHED
                    # else if we get an invalid status, same reason
                    if isinstance(response, parse.Parser):
                        result_decoded = response.parse(status.decode())
                    else:
                        result_decoded = (response == status)
                    if result_decoded is not None or serial_port.inWaiting() == 0 or ERROR_RES in status:
                        break
                attempt = attempt + 1
            if result_decoded is None:
                print_serial_status(Level.WARN, address, f'Invalid response: "{ status.decode().rstrip() if status is not None else status }" when sensing command "{ cmd.decode().rstrip() }"')
                return Status.NOT_FLASHED
            elif ERROR_RES in status:
                print_serial_status(Level.ERROR, address, f'Error status "{ status.decode().rstrip() }"')
                return Status.ERROR
            return result_decoded if isinstance(response, parse.Parser) else Status.OK
    except serial.SerialTimeoutException:
        # uh oh, looks like the serial port didn't open
        print_serial_status(Level.WARN, address, f'Hung, or not compiled with Serial.')
        return Status.HUNG

def reset_board_bootloader(address):
    # stolen from http://markparuzel.com/?p=230
    ser = serial.Serial(timeout=2, inter_byte_timeout=2)
    ser.port = address
    ser.open()
    ser.baudrate = 1200 # This is magic.
    ser.flush()
    ser.rts = True
    ser.flush()
    ser.dtr = False
    ser.flush()
    ser.close()

def upload_serial_port(bossac_path, bin_path, address):
    result = subprocess.run([bossac_path, f'--port={address}', '--force_usb_port=true', '-e', '-w', '-v', '-R', bin_path], capture_output=True)
    if result.returncode != 0:
        print_serial_status(Level.ERROR, address, f'Upload failed with error: {result.stderr}')
        return False
    print_serial_status(Level.OK, address, 'Upload succeded')
    return True

def write_config(address, config_str, fname):
    command = f'C:\'{config_str}\',\'{fname}\''
    # echo!
    print_serial_status(Level.INFO, address, f'Sending command "{command}"')
    # away we go!
    return send_serial_command(address, bytes(command + '\r\n', 'utf8'), WRITE_FILE_RES)

def flash_config(address, sub_json):
    # escape the newlines in the sub_json, and copy them into a new dictionary
    escaped_json = { k:(v.replace('\r', '\\r').replace('\n', '\\n') if isinstance(v, str) else v) for k,v in sub_json.items() }
    # send all these key/value pairs to the device
    keys_str = 'F:' + ','.join([f'\'{key}\':\'{value}\'' for key,value in escaped_json.items()])
    # echo!
    print_serial_status(Level.INFO, address, f'Sending command "{keys_str}"')
    # away we go!
    return send_serial_command(address, bytes(keys_str + '\r\n', 'utf8'), FLASH_RES)

             