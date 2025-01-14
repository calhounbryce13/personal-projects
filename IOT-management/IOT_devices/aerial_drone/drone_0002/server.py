'''
Author: Bryce Calhoun
Description: A lightweight server made with python/Flask 
            designed to represent a single IOT device
'''

from flask import Flask, request


app = Flask(__name__)

# constant port value for given device
PORT = 3001


# route handler to confirm connection between device and controller
@app.route('/connect', methods=['POST'])
def confirm_connection():
    value = request.get_json()
    return value


############################################################################################
# in a REAL-WORLD IMPLEMENTATION these functions would manipulate the hardware 
# of the device and read sensor data to get these attributes.
# since there is no 'device' in this example the sensor data is made up.

@app.route('/hover', methods=['GET'])
def drone_hover():
    altitude = 10
    speed = 0
    battery = 95
    print(f"\ndrone is hovering at an altitude of {altitude} FT and a speed of {speed} MPH, battery is now {battery}%\n")

    return {"status": "active", "altitude-ft": altitude, "speed-mph": speed, "battery-percent": battery}

@app.route('/descend', methods=['GET'])
def drone_descend():
    altitude = 0
    speed = 0
    battery = 90
    print(f"\ndrone has descended to an altitude of {altitude} FT and a speed of {speed} MPH, battery is now {battery}%\n")

    return {"status": "idle", "altitude-ft": altitude, "speed-mph": speed, "battery-percent": battery}

############################################################################################


def main():

    print(f"server running on port {PORT}!")

    # running the server app
    app.run(host='0.0.0.0', port=PORT)




if __name__ == '__main__':
    main()
    


