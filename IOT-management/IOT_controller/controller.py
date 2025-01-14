'''
Author: Bryce Calhoun
Description: An IOT device controller designed to connect to pre-defined 
            internet of things devices
'''
import random
import requests
import json


# setting arbitrary large value for wide range
TOPRANGE = 1000000

"""
this machine's IP address, would be replaced with individual
device IP addresses in REAL-WORLD IMPLEMENTATION
"""
LOCALHOST = '127.0.0.1'


        
def update_file(deviceList):
    '''
    DESCRIPTION: Function defined to update the devices.json file
                with the most accurate device state data
    INPUT(S): List of devices (list)
    OUTPUT(S): None
    '''
    with open('devices.json', mode='w') as dataFile:
        json.dump(deviceList, dataFile)
    return


def get_data_from_file():
    '''
    DESCRIPTION: Function to open the data file for the devices an return 
                its contents in a list.
    INPUT(S): None
    OUTPUT(S): Devices (list)
    '''

    with open('devices.json', mode='r') as file:
        devices = json.load(file)

    return devices


def try_connection(num, port):
    '''
    DESCRIPTION: Function to test a device connection given the port 
                it is listening on
    INPUT(S): pseudo-random number (integer), port of device (integer)
    OUTPUT(S): connection status (string)
    '''

    try:
        # making the http post request to the specific port
        response = requests.post(f"http://{LOCALHOST}:{port}/connect", json=num)
        
        if response.text == num:
            print(f"\nCONNECTED TO DEVICE ON PORT: {port}\n")
            return "True"
        else:
            print(f"\nBROKEN CONNECTION WITH PORT: {port}, RESPONSES DONT MATCH\n")
    except:
        print(f"\nBROKEN CONNECTION WITH PORT: {port}, UNABLE TO ESTABLISH CONNECTION\n")

    return "False"
        
    
def perform_action(id, action):
    '''
    DESCRIPTION: Function defined to make the accurate http request for a device action
                given the id and the action to take, as well as update the data file 
                with the updated device state
    INPUT(S): device id (string), action (string)
    OUTPUT(S): None
    '''

    port = get_port_from_id(id)
    response = requests.get(f"http://{LOCALHOST}:{port}/{action}")

    update_json(response, id)

    return


def update_json(response, id):
    '''
    DESCRIPTION: Function defined to extract the data from the response and then 
                call the correct function to update the device data file with the new information
    INPUT(S): response (json object), device id (string)
    OUTPUT(S): None
    '''

    response = response.json()
    if len(response) == 0:
        return
    
    devices = get_data_from_file()
    device = get_device_from_devices(devices, id)
    for key, value in response.items():
        device['state'][key] = str(value)

    update_file(devices)

    return

    

def get_port_from_id(id):
    '''
    DESCRIPTION: Function to help get the port of a device given it's id
    INPUT(S): device id (string)
    OUTPUT(S): device port number (string)
    '''
    devices = get_data_from_file()
    device = get_device_from_devices(devices, id)

    return device['port']


def get_device_from_devices(devices, id):
    '''
    DESCRIPTION: Function defined to return a single device from the list of devices given
                its id
    INPUT(S): list of devices (list), device id (string)
    OUTPUT(S): a single device (dictionary)
    '''
    for i in range(len(devices)):
        if devices[i]['device-id'] == str(id):
            return devices[i]

