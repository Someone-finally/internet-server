| Supported Targets | ESP32 huzzah |
| ----------------- | ----- | 

# _IoT Project using Huzzah32_
Developed an Esp32-Huzzah platform that hosts a website catering to IoT applications, ensuring sensor measurements across various commercial use cases. The website is built using Vite and leveraging the React.js library, offers a user-friendly interface. I handcrafted firmware to enable Wi-Fi functionality, sensor connectivity, and diverse communication protocols including REST, WebSockets, and MQTT—for transmitting sensor readings.


## Configurations 

Target :  Esp32-huzzah aka huzzah32

esp idf version : 5.0.1

Sensors used : 

Potentiometer : 1
LED : 1

Tilt sensor : SW-520D 
Temp & Relative Humidity sensor : SHT21 : : 12C communication required

Changes in Partition Table :

FAT Storage is set to 2MB : To host the assets of Vite website.

Total memory utilised for assets in code is 149.91 kB
Assets included: jpg

## To be implemented & Currently working 

API to access weather : Using Random API (Done)

SHT21 for temperature and humidity : I2C (Compleated code)

SD card reader : PMOD microSD 

OTA update using binaries  : Currently working on partition table and assesing the storage


## Folder contents

├── CMakeLists.txt
├── main
│   ├── CMakeLists.txt
│   └── main.c
└── README.md                  This is the file you are currently reading
```
Additionally, the sample project contains Makefile and component.mk files, used for the legacy Make based build system. 
They are not used or needed when building with CMake and idf.py.
