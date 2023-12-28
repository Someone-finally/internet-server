| Supported Targets | ESP32 huzzah |
| ----------------- | ----- | 

# _IoT Project using Huzzah32_
Esp32-huzzah that hosts a website for IoT applications for precise weather measurement for agricultural usecases . The esp32 website is created using Vite and uses React js library and the firmware for wifi, sensor connectivity is written by hand. Different protocols for communication of sensor readings such as REST, websockets, MQTT are also implemented.


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
