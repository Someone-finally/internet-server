| Supported Targets | ESP32 huzzah |
| ----------------- | ----- | 

# _IoT Project using Huzzah32_
Esp32-huzzah that hosts a website for IoT applications. The website is created using Vite and uses React js library. Different protocols for communication of sensor readings are also implemented.


## Configurations 

Target :  Esp32-huzzah aka huzzah32

esp idf version : 5.0.1

Sensors used : 

Potentiometer : 1
LED : 1

To be implemented

Tilt sensor : SW-520D 
Temp & Relative Humidity sensor : SHT21

Changes in Partition Table :

FAT Storage is set to 2MB : To host the assets of Vite website.

Total memory utilised for assets in code is 149.91 kB

## Folder contents

├── CMakeLists.txt
├── main
│   ├── CMakeLists.txt
│   └── main.c
└── README.md                  This is the file you are currently reading
```
Additionally, the sample project contains Makefile and component.mk files, used for the legacy Make based build system. 
They are not used or needed when building with CMake and idf.py.
