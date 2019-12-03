# Loom autdeployment and diagnostic script
# Works with (and only with) the AGU sketch
# Author: Noah Koontz
import click
import pyduinocli
import multiprocessing
import time
import json
from functools import partial
from util import *

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
    # reset all serial ports into booloader mode
    click.echo(f("Resetting all ports into bootloader mode...", Level.INFO))
    attempts = 0
    ports_to_reset = [address for address,pid in ports if pid != PID_BOOTLOADER]
    while attempts < 20 and len(ports_to_reset) > 0:
        if len(ports_to_reset) == 0:
            break
        for address in ports_to_reset:
            reset_board_bootloader(address)
        time.sleep(0.5)
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
    time.sleep(2)
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
    click.echo(f(f'{success} COM ports succeeded', Level.OK))
    if len(result) != len(all_ports):
        click.echo(f(f'{len(all_ports) - len(result)} COM ports excluded', Level.WARN))
    if success != len(result):
        click.echo(f(f'{len(result) - success} COM ports failed', Level.ERROR))
    return True

"""
@loom_autodeploy.command(short_help='Upload configuration to all Loom kits.')
@click.option('--ports-exclude', '-e', default=None,
              help='COM ports to exclude during the upload process')
@click.option('--arduino-cli-location', '-a', type=click.Path(exists=True, dir_okay=False), default="./arduino-cli/arduino-cli.exe",
              help='Location of the arduino-cli to use')
@click.option('--force-no-start', '-s', default=False
              help='Don\'t start the sketch after sending it to the device')
@click.argument('config', type=click.File('r', encoding="utf8"))
@click.argument('variables', type=click.File('r', encoding="utf8"))
"""
def flash(ports_exclude, arduino_cli_location, force_no_start, config, variables):
    # check that the varibles file is valid JSON
    varient_json = None
    try:
        varient_json = json.load(variables)
    except json.JSONDecodeError as e:
        click.echo(f(f'Invalid json, error: {e}', Level.ERROR))
        return False
    # get the number of ports, and check the status of each
    ports = get_serial_ports(ports_exclude)
    # get every serial port
    ports = get_serial_ports(ports_exclude)
    if len(ports) == 0:
        click.echo(f("No ports found!", Level.ERROR))
        return False
    # check the status of every serial port
    all_status = [(address, poll_serial_port(address, pid)) for address, pid in ports]
    # set all the ports into load mode, and ignore ports in bootloader mode or with an incorrect state
    load_ports = []
    for address, status in all_status:
        if status == Status.OK:
            load_ports.append(address)
        else:
            print_serial_status(Level.WARN, address, f'Ignoring port with status {status}')
    if len(load_ports) == 0:
        click.echo(f("Found no valid ports!", Level.INFO))
        return True
    # flash the config information
    click.echo(f("Sending config...", Level.INFO))
    # warn if extra ports/varients are present
    diff = len(varient_json["varients"]) - len(load_ports)
    if abs(diff) > 0:
        if diff < 0:
            click.echo(f(f'Ignoring excess ports: '
                + ', '.join([address for i,address in enumerate(load_ports) if i >= len(varient_json["varients"])]), Level.WARN))
        else:
            click.echo(f(f'Ignoring excess varients: '
                + ', '.join([json.dumps(var) for i,var in enumerate(varient_json["varients"]) if i >= len(load_ports)]), Level.WARN))
    # flash!
    now = time.ctime()
    results = []
    config_stripped = "".join(config.read().split())
    for address,varient in zip(load_ports, varient_json["varients"]):
        # merge the global JSON with the varient JSON
        device_config = {**varient_json["global"], **varient}
        device_config["stamp"] = now
        # send the config
        results.append(flash_config(address, device_config, config_stripped))
    click.echo(f("Flash done! restarting...", Level.OK))
    # start the sketch, if requested
    if force_no_start != True:
        for i,address in enumerate(load_ports):
            # for every device that succeded, start it, and fail if it fails to start
            if i < len(varient_json["varients"]) and results[i] == Status.OK:
                if send_serial_command(address, START_LOOM, START_RES) != Status.OK:
                    results[i] = False
    # print the results!
    success = sum([1 for r in results if r == Status.OK])
    click.echo(f(f'{success} COM ports succeded', Level.OK))
    if abs(diff) > 0:
        if diff < 0:
            click.echo(f(f'{abs(diff)} COM ports excluded', Level.WARN))
        else:
            click.echo(f(f'{diff} varients excluded', Level.WARN))
    possible_ports = min(len(varient_json["varients"]), len(load_ports))
    if success != possible_ports:
        click.echo(f(f'{possible_ports - success} COM ports failed', Level.ERROR))
    time.sleep(1)
    # check the status of every serial port
    for address,pid in get_serial_ports(ports_exclude):
        poll_serial_port(address, pid)
    return True


@loom_autodeploy.command(short_help='Run diagnostics on all Loom kits.')
@click.option('--ports-exclude', '-e', default=None,
              help='COM ports to exclude during the upload process')
@click.option('--arduino-cli-location', '-a', type=click.Path(exists=True, dir_okay=False), default="./arduino-cli/arduino-cli.exe",
              help='Location of the arduino-cli to use') 
@click.argument('config', type=click.File('r', encoding='utf8'))
def diagnose(port_exclude, cli_path, config):
    """
    Run diagnostics on Loom kits
    """
    pass

if __name__ == '__main__':
    # loom_autodeploy()
    # upload([], False, "./arduino-cli/arduino-cli.exe", "./bossac/bossac.exe", "mock.bin", False, "mock-sketch", "../config.json")
    upload([], False, "./arduino-cli/arduino-cli.exe", "./bossac/bossac.exe", "Loom.bin", False, "../../Base", "../config.json")
    time.sleep(1)
    flash([], "./arduino-cli/arduino-cli.exe", False, open("../HydroKitConfig.json", "r", encoding="utf8"),  open("../HydroKitVarients.json", "r", encoding="utf8"))