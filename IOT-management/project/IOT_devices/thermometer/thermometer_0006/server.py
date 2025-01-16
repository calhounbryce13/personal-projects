'''
Author: Bryce Calhoun
Description: A lightweight server made with python/Flask 
            designed to represent a single IOT device
'''

from flask import Flask, request


app = Flask(__name__)

# constant port value for given device
PORT = 3005


# route handler to confirm connection between device and controller
@app.route('/connect', methods=['POST'])
def confirm_connection():
    value = request.get_json()
    return value


############################################################################################
# in a REAL-WORLD IMPLEMENTATION these functions would manipulate the hardware 
# of the device and read sensor data to get these attributes.
# since there is no 'device' in this example the sensor data is made up.

@app.route('/activate', methods=['GET'])
def begin_task():
    
    battery = 99
    temperature = 73
    print(f"\nThermometer is now collecting data !\n")

    return {"status": "active", "battery-percent": battery, "temperature-F": temperature}

@app.route('/de-activate', methods=['GET'])
def end_task():
    
    battery = 67
    temperature = ''
    print(f"\nThermometer is not collecting data anymore\n")

    return {"status": "idle", "battery-percent": battery, "temperature-F": temperature}

############################################################################################


def main():

    print(f"server running on port {PORT}!")

    # running the server app
    app.run(host='0.0.0.0', port=PORT)




if __name__ == '__main__':
    main()
    


