# Loom autdeployment and diagnostic script
# Works with (and only with) the AGU sketch
# Author: Noah Koontz
import click
import pyduinocli
import serial
import parse
import enum
from functools import partial
from colorama import init, Fore, Back
import multiprocessing
import subprocess
import time

init(autoreset=True)

FQBN = "adafruit:samd:adafruit_feather_m0"
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
    HUNG = 3

class Level(enum.Enum):
    OK = Back.GREEN + Fore.BLACK
    INFO = Back.RESET + Fore.RESET
    WARN = Back.YELLOW + Fore.BLACK
    ERROR = Back.RED + Fore.BLACK

# functions to find and check serial ports
def build_sketch(cli, bin_path, sketch):
    """./arduino-cli/arduino-cli.exe compile -b adafruit:samd:adafruit_feather_m0 -o loom.bin ../../Base"""
    return cli.compile(sketch, fqbn=BOARD, output=bin_path)

def f(printed, level):
    return str(level.value) + printed + str(Level.INFO.value)

def get_serial_ports(cli, exclude):
    boards_all = cli.board.list()
    print(boards_all)
    boards_filtered = filter(lambda board: 
        board["address"] not in exclude and board["boards"][0]["FQBN"] is FQBN, boards_all)
    return list(map(lambda board: board["address"], boards_all))

def poll_serial_port(address):
    try:
        with serial.Serial(address, BAUD, timeout=2, write_timeout=2, inter_byte_timeout=2) as serial_port:
            # poll the device
            serial_port.write(STATUS)
            status = None
            # if the read times out, the device probably just isn't flashed
            try:
                status = serial_port.readline().decode()
            except serial.SerialTimeoutException:
                click.echo(f'{ f(address, Level.WARN) }: No status from port')
                return Status.NOT_FLASHED
            # else if we get an invalid status, same reason
            result = STATUS_RES.parse(status)
            if result is None:
                click.echo(f'{ f(address, Level.WARN) }: Invalid status: "{ status.rstrip() }"')
                return Status.NOT_FLASHED
            # else the device is flashed! check the status
            click.echo(f'{ f(address, Level.OK) }: Mode: '
                + ("Bootloader" if result["mode_boot"] else "Sketch")
                + f', Built: { result["build_time"] }, Boot: { result["boot_time"] }')
            if result["mode_boot"]:
                return Status.BOOTLOADER
            else:
                return Status.OK
    except serial.SerialTimeoutException:
        # uh oh, looks like the serial port didn't open
        click.echo(f'{ f(address, Level.WARN) }: Hung, or not compiled with Serial.')
        return Status.HUNG

def reset_board_bootloader(address):
    # stolen from http://markparuzel.com/?p=230
    ser = serial.Serial(timeout=2, inter_byte_timeout=2)
    ser.port = address
    ser.open(); 
    ser.baudrate = 1200 # This is magic.
    ser.rts = True
    ser.dtr = False
    ser.close()

def upload_serial_port(bossac_path, bin_path, address):
    click.echo(f'{ f(address, Level.INFO) }: Starting upload...')
    result = subprocess.run([bossac_path, f'--port={address}', '--force_usb_port=true', '-e', '-w', '-v', '-R', bin_path], capture_output=True)
    if result.returncode is not 0:
        click.echo(f'{ f(address, Level.ERROR) }: Upload failed with error: {result.stderr}')
        return False
    click.echo(f'{ f(address, Level.OK) }: Upload succeded')
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
@click.argument('sketch', type=click.File('r'))
@click.argument('config', type=click.File('r'))
"""
def upload(ports_exclude, disable_diagnostics, cli_path, bossac_path, bin_path, sketch, config):
    """ 
    Upload and diagnose a bunch of Loom kits.
    """
    ARDUINO = pyduinocli.Arduino(cli_path)
    if ARDUINO is None:
        click.echo(f("Invalid Arduino CLI path!", Level.ERROR))
        return
    # get every serial port
    ports = get_serial_ports(ARDUINO, ports_exclude)
    # for port in ports:
    #     poll_serial_port(port)
    click.echo(f("Uploading sketch to all ports...", Level.INFO))
    # reset every board into bootloader mode
    for port in ports:
        reset_board_bootloader(port)
    time.sleep(1)
    # find all the ports created from that, and use those as upload ports
    upload_ports = get_serial_ports(ARDUINO, ports_exclude)
    # upload to all devices in parellel
    with multiprocessing.Pool(processes=5) as pool:
        upload_bound = partial(upload_serial_port, bossac_path, bin_path)
        async_tasks = []
        for port in upload_ports:
            async_tasks.append(pool.apply_async(upload_bound, (port,)))
        result = list(map(lambda task: task.get(15), async_tasks))
        print(result)

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
    upload([], False, "./arduino-cli/arduino-cli.exe", "./bossac/bossac.exe", "./loom.bin", "../Base.ino", "../config.json")