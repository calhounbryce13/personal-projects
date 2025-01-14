'''
Author: Bryce Calhoun
Description: A GUI made with python/kivy for the user to interact
            with the IOT controller
'''


#######################################################################
#imports
import controller as Controller
import random
from kivy.app import App
from kivy.lang import Builder
from kivy.uix.screenmanager import Screen, ScreenManager
from kivy.core.window import Window
from kivy.uix.button import Button
from kivy.uix.label import Label
from kivy.uix.boxlayout import BoxLayout
from kivy.properties import ObjectProperty, StringProperty, ListProperty
#######################################################################


Window.clearcolor = (1,1,1,1)

class MyApp(App):
    """
    DESCRIPTION: Class definition for the applicaiton instance,
                initiallizes my screen manager with my two screens
    """

    def build(self):
        '''
        DESCRIPTION: the build function for the application, loads the kvlang file
                    and initializes the screen manager
        INPUT(S): None
        OUTPUT(S): ScreenManager instance
        '''

        kv = Builder.load_file('design.kv')

        self.sm = ScreenManager()
        self.sm.add_widget(HubScreen(name='hub'))
        self.sm.add_widget(DeviceMonitor(name='monitor'))

        return self.sm
    

class DeviceButton(Button):
    """
    DESCRIPTION: A class definition for a custom button for each device representation this allows me to 
                dynamically add background image and connection status color
    """
    backgroundImage = StringProperty('')
    overlayColor = ListProperty([0,0,0,0])


    def navigate_to_monitor(self):
        '''
        DESCRIPTION: Function defined to BOTH 1. set the monitor screen variable to the device id selected
                    and 2. navigate user to the monitor screen
        INPUT(S): None
        OUTPUT(S): None
        '''
    
        self.parent.parent.parent.manager.get_screen('monitor').deviceId = self.text
        self.parent.parent.parent.manager.current = 'monitor'

        return


class DeviceMonitor(Screen):
    """
    DESCRIPTION: Class definition for the device monitoring screen within the application
                each device uses the same screen with dynamically added data
    """

    deviceId = StringProperty('')
    def __init__(self, **kw):
        super(DeviceMonitor, self).__init__(**kw)


    def back(self):
        '''
        DESCRIPTION: Simple navigation back to the central hub view
        INPUT(S): None
        OUTPUT(S): None
        '''

        self.manager.current = 'hub'

        return
    

    def populate_data_section(self, myDevice):
        '''
        DESCRIPTION: Function defined to populate the section in the monitor screen 
                    designated to display device state info
        INPUT(S): given device (dictionary)
        OUTPUT(S): None
        '''

        dataSection = self.ids['dataSection']
        for key, value in myDevice['state'].items():

            statusLabel = Label(text=f'{key}'.upper(), size_hint=(0.3, 1), color=(0,0,0,1))
            statusValue = Label(text=f'{value}'.lower(), size_hint=(None, 1), color=(0,0,0,1))

            dataLayout = BoxLayout(orientation='horizontal')

            dataLayout.add_widget(statusLabel)
            dataLayout.add_widget(statusValue)

            dataSection.add_widget(dataLayout)

        return


    def populate_action_section(self, myDevice):
        '''
        DESCRIPTION: Function defined to populate the section in the monitor screen 
                    designated to display device functionality buttons
        INPUT(S): given device (dictionary)
        OUTPUT(S): None
        '''

        actionSection = self.ids['actionSection']
        for i in range(len(myDevice['actions'])):
            
            myAction = Button(text=f'{myDevice['actions'][i]}'.upper(), size_hint=(1, 1))
            myAction.bind(on_press=lambda instance, i=i: self.on_action_press(myDevice['actions'][i]))
            actionSection.add_widget(myAction)

        return
    

    def on_action_press(self, action):
        '''
        DESCRIPTION: Function defined to call appropriate controller 
                    functions based on the device id and action to perfrom 
                    the task and update the UI.
        INPUT(S): given action (string)
        OUTPUT(S): None
        '''

        Controller.perform_action(self.deviceId, action)
        self.clear_data_section()

        devices = Controller.get_data_from_file()
        myDevice = self.return_device_from_json(devices, self.deviceId)
        self.populate_data_section(myDevice)

        return


    def on_pre_enter(self):
        '''
        DESCRIPTION: Function defined to call the appropriate controller
                    functions to set up the monitor screen with accurate data.
        INPUT(S): None
        OUTPUT(S): None
        '''

        devices = Controller.get_data_from_file()
        myDevice = self.return_device_from_json(devices, self.deviceId)
        titleLabel = self.ids['myDeviceLabel']
        titleLabel.text = (myDevice['type'] + ' ' + myDevice['device-id']).capitalize()

        self.populate_data_section(myDevice)
        self.populate_action_section(myDevice)

        return
    

    def clear_data_section(self):
        '''
        DESCRIPTION: Function defined to erase the current data from
                    the data display section.
        INPUT(S): None
        OUTPUT(S): None
        '''
        dataSection = self.ids['dataSection']
        dataSection.clear_widgets()

        return


    def clear_action_section(self):
        '''
        DESCRIPTION: Function defined to erase the current data from
                    the functionality buttons section.
        INPUT(S): None
        OUTPUT(S): None 
        '''
        actionSection = self.ids['actionSection']
        actionSection.clear_widgets()

        return

    
    def on_pre_leave(self):
        '''
        DESCRIPTION: Function defined to clear the device monitor screen
                    of the current device data and functionality
        INPUT(S): None
        OUTPUT(S): None
        '''
        self.clear_data_section()
        self.clear_action_section()

        return

    
    def return_device_from_json(self, devicesArray, id):
        '''
        DESCRIPTION: Function defined to return a single device
                    from a list of devices given the id of the device
        INPUT(S): list of devices (list), device id (string)
        OUTPUT(S): a single device (dictionary)
        '''

        for i in range(len(devicesArray)):
            if devicesArray[i]['device-id'] == id:
                return devicesArray[i]
            
        return False


class HubScreen(Screen):
    """
    DESCRIPTION: Class definition for the main 'hub' of the application
    """

    device_1 = ObjectProperty(None)
    device_2 = ObjectProperty(None)
    device_3 = ObjectProperty(None)
    device_4 = ObjectProperty(None)
    device_5 = ObjectProperty(None)
    device_6 = ObjectProperty(None)
    

    def __init__(self, **kwargs):
        super(HubScreen, self).__init__(**kwargs)


    def connect(self):
        '''
        DESCRIPTION: Function defined to connect to each of the devices on
                    the network in order to establish connection status.    
        INPUT(S): None
        OUTPUT(S): None
        '''
        devices = Controller.get_data_from_file()

        for x in range(len(devices)):
            num = str(random.randint(0, Controller.TOPRANGE))   
            port = devices[x]['port']
            devices[x]['connected'] = Controller.try_connection(num, port)

            button = self.ids[f"device_{x+1}"]
            button.text = (devices[x]['device-id'])
            button.font_size = 20

            self.establish_device_representation(devices, x, button)
        Controller.update_file(devices)

        return


    def establish_device_representation(self, devices, x, button):
        '''
        DESCRIPTION: Function defined to set the device images and set their overlay color
        INPUT(S): list of devices (list), counter variable (integer), button instance (Button class)
        OUTPUT(S): None
        '''
        deviceType = devices[x]['type']
        self.set_device_images(deviceType, button)

        connectionStatus = devices[x]['connected']
        self.set_connection_status(connectionStatus, button)

        return


    def set_device_images(self, deviceType, button):
        '''
        DESCRIPTION: Function defined to set one of three device class images on 
                    the given button widget
        INPUT(S): device type (string), button instance (Button class)
        OUTPUT(S): None
        '''
            
        if deviceType == 'aerial-drone':
            button.backgroundImage = 'icons/icons8-drone-100.png'
        elif deviceType == 'factory-robot':
            button.backgroundImage = 'icons/icons8-industrial-robot-100.png'
        else:
            button.backgroundImage = 'icons/icons8-thermometer-100.png'

        return


    def set_connection_status(self, connectionStatus, button):
        '''
        DESCRIPTION: Function defined to place either a green or red 
                    overlay over the button image to show wether the device id responding or not.
        INPUT(S): current connection status (string), button instance (Button class)
        OUTPUT(S): None
        '''
        if connectionStatus == 'False':
            button.overlayColor = [1, 0, 0, 0.4]
        else:
            button.overlayColor = [0, 1, 0, 0.4]     

        return    


def main():
    MyApp().run()


if __name__ == '__main__':
    main()
