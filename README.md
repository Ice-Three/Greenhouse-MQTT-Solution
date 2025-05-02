# Greenhouse Temperature Control

This is produced as a school project at [Novia University of Applied Sciences](https://www.novia.fi/en/) in Finland as a part of our course on Embedded Systems.
The goal of this project has been to create an MQTT Solution to manage 3 greenhouses with 1 data aggregator each as well as five data collectors each.
The focus for this has to make the project easily expandable for multiple usage cases with minimal coding needed to expand the project however needed.

# Table of Contents
- [Requirements](#Requirements)
  - [Hardware](##Hardware)
  - [Software](##Software)
      - [Packages](##Packages)
- [Installation](#Installation)
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

## Software
**Collectors**
- [Arduino IDE](https://www.arduino.cc/en/software/) or other ESP32 / Arduino Capable Editors

**Aggregators**
- [RaspberryOS](https://www.raspberrypi.com/software/)
- [Node-RED](https://nodered.org)**
- [Mosquitto](https://mosquitto.org)**

**These packages can be installed through RaspberryOS CLI.

**Client**
- [Docker](https://www.docker.com) (Optional but Recommended)
- [Node-RED](https://nodered.org)

_Docker is recommended because it is capable of running a container of Node-RED which assists in reliable upkeep_
### Packages
**Collectors**
- [PubSubClient](https://github.com/knolleary/pubsubclient)
- [Adafruit Unified Sensor Driver](https://github.com/adafruit/Adafruit_Sensor)***
- [Adafruit AM2320 Sensor Driver for Arduino](https://github.com/adafruit/Adafruit_AM2320)***

***These packages can be installed through Arduino IDE Library manager.

**Aggregators**
-[Node-Red-node-pi-gpio](https://flows.nodered.org/node/node-red-node-pi-gpio)
  - [@flowfuse/node-red-dashboard](https://flows.nodered.org/node/@flowfuse/node-red-dashboard)****
  - [@flowfuse/node-red-dashboard-2-ui-led](https://flows.nodered.org/node/@flowfuse/node-red-dashboard-2-ui-led)****
  - [node-red-dashboard](https://flows.nodered.org/node/node-red-dashboard)****

****Not necessary for core functionality, but strongly recommended for local debugging.

**Client**
- [@flowfuse/node-red-dashboard](https://flows.nodered.org/node/@flowfuse/node-red-dashboard)
- [@flowfuse/node-red-dashboard-2-ui-led](https://flows.nodered.org/node/@flowfuse/node-red-dashboard-2-ui-led)
- [node-red-contrib-whatsapp-link](https://flows.nodered.org/node/node-red-contrib-whatsapp-link)

# Installation

# Usage
