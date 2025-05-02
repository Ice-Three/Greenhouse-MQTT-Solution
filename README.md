# Greenhouse Control Center

This is produced as a school project at [Novia University of Applied Sciences](https://www.novia.fi/en/) in Finland as a part of our course on Embedded Systems.
The goal of this project has been to create an MQTT Solution to manage 3 greenhouses with 1 data aggregator each as well as five data collectors each.
The project was done with the intent to be easily scalable with minimal coding required to deploy the system.
While the project is scalable and the code has been made to be scalable. The restraints of this as a school project means the project has only been tested with a singular aggregator and a singular collector.

# Table of Contents
- [Requirements](#Requirements)
  - [Hardware](##Hardware)
  - [Software](##Software)
      - [Packages](##Packages)
- [Installation](#Installation)
  - [Node-RED](##Node-RED)
- [Usage](#Usage)

# Requirements
## Hardware
**Collectors**
- ESP32
- Adafruit AM2320*
- BPT-14 Piezo Buzzer*
- LED5MM-3C RGB*
- Fan*

*Alterantives can be used but code may change.

**Aggregators**
- Raspberry Pi 3 or Higher

**MQTT Cloud Server**

⚠️This project requires being able to connect to an MQTT cloud server for IoT usage. This project does not cover how to create or manage one. Such must be done before the project can work. ⚠️

## Software
**Collectors**
- [Arduino IDE](https://www.arduino.cc/en/software/) or other ESP32 / Arduino Capable Editors

**Aggregators**
- [RaspberryOS](https://www.raspberrypi.com/software/)
- [Node-RED](https://nodered.org)
- [Mosquitto](https://mosquitto.org)


**Client**
- [Docker](https://www.docker.com) (Optional but Recommended)
- [Node-RED](https://nodered.org)

_Docker is recommended because it is capable of running a container of Node-RED which assists in reliable upkeep_
### Packages
**Collectors**
- [PubSubClient](https://github.com/knolleary/pubsubclient)
- [Adafruit Unified Sensor Driver](https://github.com/adafruit/Adafruit_Sensor)**
- [Adafruit AM2320 Sensor Driver for Arduino](https://github.com/adafruit/Adafruit_AM2320)**

**These packages can be installed through Arduino IDE Library manager.

**Aggregators**

- [Node-Red-node-pi-gpio](https://flows.nodered.org/node/node-red-node-pi-gpio)
  - [@flowfuse/node-red-dashboard](https://flows.nodered.org/node/@flowfuse/node-red-dashboard)***
  - [@flowfuse/node-red-dashboard-2-ui-led](https://flows.nodered.org/node/@flowfuse/node-red-dashboard-2-ui-led)***
  - [node-red-dashboard](https://flows.nodered.org/node/node-red-dashboard)****

***Not necessary for core functionality, but strongly recommended for local debugging.

**Client**

- [@flowfuse/node-red-dashboard](https://flows.nodered.org/node/@flowfuse/node-red-dashboard)
- [@flowfuse/node-red-dashboard-2-ui-led](https://flows.nodered.org/node/@flowfuse/node-red-dashboard-2-ui-led)
- [node-red-contrib-whatsapp-link](https://flows.nodered.org/node/node-red-contrib-whatsapp-link)

# Installation
**Collectors**

<ins> Arduino IDE </ins>
- Include all packages in Library folder
- Open Code Sketch in Arduino IDE
- Edit SSID and Password to match the network. Note: The Collector and Aggregator must be on the same network if Portforwarding is not done.
- Add the IP of the Aggregator as the mqtt_server variable.
- Adjust the Collector ID to define the collector within the system.

**Aggregators**
- Install RaspberryOS
- Install Node-RED and Mosquitto
  
  ``sudo apt update && sudo apt upgrade``

  ``apt-get install nodered``

  ``sudo apt install -y mosquitto mosquitto-clients``
- Enable Node-RED && Mosquitto on boot
  ``sudo systemctl enable nodered.service``

  ``sudo systemctl enable mosquitto.service``
  
  - Enable Remote Access for Mosquitto Broker

    ``sudo nano /etc/mosquitto/conf.d/RemoteAccess``
    
    Add to the file the following:

    ``listener 1883``
      
    ``allow_anonymous true``

    Then run the following command after the file has been saved:

    ``sudo systemctl restart mosquitto``

  This is required as to allow remote access as to get Mosquitto to work properly. If you wish to add remote access with user and password authentication, you may find such [here](https://randomnerdtutorials.com/how-to-install-mosquitto-broker-on-raspberry-pi/).
  The code is however not produced with username and password authentication in mind.

**Client**

This will not go through the process of running Node-Red on docker.
However, the required information can be found [here](https://nodered.org/docs/getting-started/docker).
It is also possible to run Node-RED from Docker Desktop.

## Node-RED
**Installation of Node-RED Packages**

This can be done by navigating to ``Manage palette``
Under the ``Install`` tab all the required packages can be found.

**Importing Node-RED**

This can be done by navigating to ``import`` this can also be done with ``Ctrl-i``.
There you can either paste the JSON flow or Import from File.

**Editing Node-RED**

Once installed and imported. Navigate to; ``local`` and adjust ``count`` if required.

Select a ``mqqt`` and double click the node. Edit the ``mqtt server`` where you are able to connect to your MQTT Cloud Server.
# Usage
Once everything has been installed and is up and running. Navigate to:

``[Client-IP]:[Port]/Dashboard`` (Change the IP and Port to the correct values. Default port is 1880)

Navigate to ``Nominal Values`` & ``Initialize Local Database``

Your system should now work and you should be able to freely adjust the nominal values.
