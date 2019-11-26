# Loom autdeployment and diagnostic script
# Works with (and only with) the AGU sketch
# Author: Noah Koontz
import click
import pyduinocli
import serial
from serial.tools.list_ports import comports
import parse
import enum
from functools import partial
from colorama import init, Fore, Back
import multiprocessing
import subprocess
import threading
import time

init(autoreset=True)

FQBN = "adafruit:samd:adafruit_feather_m0"
PID_SKETCH = 32779
PID_BOOTLOADER = 11
VID = 9114
BAUD = 115200

STATUS = b'R\r\n'
DIAGNOSE = b'D\r\n'
# configuration (JSON object), Spool coordinator ID, Spool device ID, Spool public key, Spool private key
FLASH = 'F:"{json}","{coord_id}","{dev_id}","{pub_key}","{priv_key}"\r\n'
START_LOOM = b'S\r\n'

# F:OK
FLASH_RES = b'F:OK'
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
    LOAD_READY = 3
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
                    return Status.LOAD_READY
                else:
                    return Status.OK
        else:
            print_serial_status(Level.ERROR, address, f'Unknown PID {pid}')
            return Status.HUNG
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

# Click setup and commands:
@click.group()
def loom_autodeploy():
    """OPEnS AGU Upload and Diagnostic Tool
    Automatically uploads and configures AGU Loom Kits as fast as possible.
    """
    pass
"""
@loom_autodeploy.command(short_help='Upload the Base sketch and configuration to a Loom Kit, and run diagnostics on all kits.')
@click.option('--ports-exclude', '-e', default=[], multiple=True,
              help='COM ports to exclude during the upload process')
@click.option('--disable-diagnostics', '-d', default=False,
              help='Disable running diagnostics for the Loom kits.') 
@click.option('--cli-path', '-a', type=click.Path(exists=True, dir_okay=False), default="./arduino-cli/arduino-cli.exe",
              help='Location of the arduino-cli to use')
@click.option('--bossac-path', '-u', type=click.Path(exists=True, dir_okay=False), default="./bossac/bossac.exe",
              help='Location of the BOSSA uploader tool to use')
@click.option('--bin-path', '-b', type=click.Path(dir_okay=False), default="sketch.bin",
    help='Location to place the temporary built sketch.')
@click.option('--disable-build', '-n', default=False,
    help='Force using the cached build binary, instead of rebuilding the sketch') 
@click.argument('sketch', type=click.File('r'))
@click.argument('config', type=click.File('r'))
"""
def upload(ports_exclude, disable_diagnostics, cli_path, bossac_path, bin_path, disable_build, sketch, config):
    """ 
    Upload and diagnose a bunch of Loom kits.
    """
    ARDUINO = pyduinocli.Arduino(cli_path)
    if ARDUINO == None:
        click.echo(f("Invalid Arduino CLI path!", Level.ERROR))
        return
    # build the sketch if requested
    if disable_build != True:
        click.echo(f("Building sketch...", Level.INFO))
        if build_sketch(ARDUINO, bin_path, sketch) != True:
            click.echo(f(f'Error building sketch: {e}', Level.ERROR))
            return False
        click.echo(f("Done building sketch!", Level.OK))
    # get every serial port
    click.echo(f("Polling all serial ports...", Level.INFO))
    ports = get_serial_ports(ports_exclude)
    if len(ports) == 0:
        click.echo(f("No ports found!", Level.ERROR))
    # check the status of every serial port
    for address,pid in ports:
        poll_serial_port(address, pid)
    # reset all serial ports into booloader mode
    click.echo(f("Resetting all ports into bootloader mode...", Level.INFO))
    attempts = 0
    ports_to_reset = [address for address,pid in ports if pid != PID_BOOTLOADER]
    while attempts < 5 and len(ports_to_reset) > 0:
        if len(ports_to_reset) == 0:
            break
        for address in ports_to_reset:
            reset_board_bootloader(address)
        time.sleep(2)
        ports_to_reset = [address for address,pid in get_serial_ports(ports_exclude) if pid != PID_BOOTLOADER]
        attempts = attempts + 1
    # tell the user to manually reset all the devices that didn't take for some reason
    if len(ports_to_reset) > 0:
        click.echo(f(f'Some ports did not reset! please manually reset ports {", ".join(ports_to_reset)}', Level.WARN))
        click.pause()
    else:
        click.echo(f('All ports were reset!', Level.OK))
    # upload all the sketch to all the ports
    click.echo(f("Uploading sketch to all ports...", Level.INFO))
    attempts = 0
    # find all the ports created from that, and use those as upload ports
    all_ports = get_serial_ports(ports_exclude)
    upload_ports = []
    for address,pid in all_ports:
        if pid != PID_BOOTLOADER:
            print_serial_status(Level.WARN, address, f'excluded due to invalid PID {pid}')
        else:
            upload_ports.append(address)
    # upload to all devices in parellel
    result = []
    with multiprocessing.Pool(processes=10) as pool:
        upload_bound = partial(upload_serial_port, bossac_path, bin_path)
        async_tasks = []
        for address in upload_ports:
            print_serial_status(Level.INFO, address, 'Starting upload...')
            async_tasks.append(pool.apply_async(upload_bound, (address,)))
        result = [task.get(15) for task in async_tasks]
    # print the result
    success = sum([1 for r in result if r == True])
    click.echo(f(f'{success} COM ports succeded', Level.OK))
    if len(result) != len(all_ports):
        click.echo(f(f'{len(all_ports) - len(result)} COM ports excluded', Level.WARN))
    if success != len(result):
        click.echo(f(f'{len(result) - success} COM ports failed', Level.ERROR))
    time.sleep(1)
    # check the status of every serial port
    for address,pid in get_serial_ports(ports_exclude):
        poll_serial_port(address, pid)

@loom_autodeploy.command(short_help='Upload configuration to all Loom kits.')
@click.option('--ports-exclude', '-e', default=None,
              help='COM ports to exclude during the upload process')
@click.option('--arduino-cli-location', '-a', type=click.Path(exists=True, dir_okay=False), default="./arduino-cli/arduino-cli.exe",
              help='Location of the arduino-cli to use')
@click.argument('config', type=click.File('r'))
@click.argument('variables', type=click.File('r'))
def flash()


@loom_autodeploy.command(short_help='Run diagnostics on all Loom kits.')
@click.option('--ports-exclude', '-e', default=None,
              help='COM ports to exclude during the upload process')
@click.option('--arduino-cli-location', '-a', type=click.Path(exists=True, dir_okay=False), default="./arduino-cli/arduino-cli.exe",
              help='Location of the arduino-cli to use') 
@click.argument('config', type=click.File('r'))
def diagnose(port_exclude, cli_path, config):
    """
    Run diagnostics on Loom kits
    """
    pass

if __name__ == '__main__':
    # loom_autodeploy()
    upload([], False, "./arduino-cli/arduino-cli.exe", "./bossac/bossac.exe", "./mock.bin", False, "mock-sketch", "../config.json")