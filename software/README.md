# HixCO2TemperatureIRBlaster - Firmware

![Build with PlatformIO](https://img.shields.io/badge/build%20with-PlatformIO-orange?logo=data%3Aimage%2Fsvg%2Bxml%3Bbase64%2CPHN2ZyB3aWR0aD0iMjUwMCIgaGVpZ2h0PSIyNTAwIiB2aWV3Qm94PSIwIDAgMjU2IDI1NiIgeG1sbnM9Imh0dHA6Ly93d3cudzMub3JnLzIwMDAvc3ZnIiBwcmVzZXJ2ZUFzcGVjdFJhdGlvPSJ4TWlkWU1pZCI+PHBhdGggZD0iTTEyOCAwQzkzLjgxIDAgNjEuNjY2IDEzLjMxNCAzNy40OSAzNy40OSAxMy4zMTQgNjEuNjY2IDAgOTMuODEgMCAxMjhjMCAzNC4xOSAxMy4zMTQgNjYuMzM0IDM3LjQ5IDkwLjUxQzYxLjY2NiAyNDIuNjg2IDkzLjgxIDI1NiAxMjggMjU2YzM0LjE5IDAgNjYuMzM0LTEzLjMxNCA5MC41MS0zNy40OUMyNDIuNjg2IDE5NC4zMzQgMjU2IDE2Mi4xOSAyNTYgMTI4YzAtMzQuMTktMTMuMzE0LTY2LjMzNC0zNy40OS05MC41MUMxOTQuMzM0IDEzLjMxNCAxNjIuMTkgMCAxMjggMCIgZmlsbD0iI0ZGN0YwMCIvPjxwYXRoIGQ9Ik0yNDkuMzg2IDEyOGMwIDY3LjA0LTU0LjM0NyAxMjEuMzg2LTEyMS4zODYgMTIxLjM4NkM2MC45NiAyNDkuMzg2IDYuNjEzIDE5NS4wNCA2LjYxMyAxMjggNi42MTMgNjAuOTYgNjAuOTYgNi42MTQgMTI4IDYuNjE0YzY3LjA0IDAgMTIxLjM4NiA1NC4zNDYgMTIxLjM4NiAxMjEuMzg2IiBmaWxsPSIjRkZGIi8+PHBhdGggZD0iTTE2MC44NjkgNzQuMDYybDUuMTQ1LTE4LjUzN2M1LjI2NC0uNDcgOS4zOTItNC44ODYgOS4zOTItMTAuMjczIDAtNS43LTQuNjItMTAuMzItMTAuMzItMTAuMzJzLTEwLjMyIDQuNjItMTAuMzIgMTAuMzJjMCAzLjc1NSAyLjAxMyA3LjAzIDUuMDEgOC44MzdsLTUuMDUgMTguMTk1Yy0xNC40MzctMy42Ny0yNi42MjUtMy4zOS0yNi42MjUtMy4zOWwtMi4yNTggMS4wMXYxNDAuODcybDIuMjU4Ljc1M2MxMy42MTQgMCA3My4xNzctNDEuMTMzIDczLjMyMy04NS4yNyAwLTMxLjYyNC0yMS4wMjMtNDUuODI1LTQwLjU1NS01Mi4xOTd6TTE0Ni41MyAxNjQuOGMtMTEuNjE3LTE4LjU1Ny02LjcwNi02MS43NTEgMjMuNjQzLTY3LjkyNSA4LjMyLTEuMzMzIDE4LjUwOSA0LjEzNCAyMS41MSAxNi4yNzkgNy41ODIgMjUuNzY2LTM3LjAxNSA2MS44NDUtNDUuMTUzIDUxLjY0NnptMTguMjE2LTM5Ljc1MmE5LjM5OSA5LjM5OSAwIDAgMC05LjM5OSA5LjM5OSA5LjM5OSA5LjM5OSAwIDAgMCA5LjQgOS4zOTkgOS4zOTkgOS4zOTkgMCAwIDAgOS4zOTgtOS40IDkuMzk5IDkuMzk5IDAgMCAwLTkuMzk5LTkuMzk4em0yLjgxIDguNjcyYTIuMzc0IDIuMzc0IDAgMSAxIDAtNC43NDkgMi4zNzQgMi4zNzQgMCAwIDEgMCA0Ljc0OXoiIGZpbGw9IiNFNTcyMDAiLz48cGF0aCBkPSJNMTAxLjM3MSA3Mi43MDlsLTUuMDIzLTE4LjkwMWMyLjg3NC0xLjgzMiA0Ljc4Ni01LjA0IDQuNzg2LTguNzAxIDAtNS43LTQuNjItMTAuMzItMTAuMzItMTAuMzItNS42OTkgMC0xMC4zMTkgNC42Mi0xMC4zMTkgMTAuMzIgMCA1LjY4MiA0LjU5MiAxMC4yODkgMTAuMjY3IDEwLjMxN0w5NS44IDc0LjM3OGMtMTkuNjA5IDYuNTEtNDAuODg1IDIwLjc0Mi00MC44ODUgNTEuODguNDM2IDQ1LjAxIDU5LjU3MiA4NS4yNjcgNzMuMTg2IDg1LjI2N1Y2OC44OTJzLTEyLjI1Mi0uMDYyLTI2LjcyOSAzLjgxN3ptMTAuMzk1IDkyLjA5Yy04LjEzOCAxMC4yLTUyLjczNS0yNS44OC00NS4xNTQtNTEuNjQ1IDMuMDAyLTEyLjE0NSAxMy4xOS0xNy42MTIgMjEuNTExLTE2LjI4IDMwLjM1IDYuMTc1IDM1LjI2IDQ5LjM2OSAyMy42NDMgNjcuOTI2em0tMTguODItMzkuNDZhOS4zOTkgOS4zOTkgMCAwIDAtOS4zOTkgOS4zOTggOS4zOTkgOS4zOTkgMCAwIDAgOS40IDkuNCA5LjM5OSA5LjM5OSAwIDAgMCA5LjM5OC05LjQgOS4zOTkgOS4zOTkgMCAwIDAtOS4zOTktOS4zOTl6bS0yLjgxIDguNjcxYTIuMzc0IDIuMzc0IDAgMSAxIDAtNC43NDggMi4zNzQgMi4zNzQgMCAwIDEgMCA0Ljc0OHoiIGZpbGw9IiNGRjdGMDAiLz48L3N2Zz4=)
![Software released](https://img.shields.io/badge/SW-released-green)
![Hardward](https://img.shields.io/badge/HW-prototype-red)


## How to flash the device

### Step 0: software confirmation
* make sure you have set your WIFI credentials in the secret.cpp
* make sure you have PlatformIO correctly setup in your Microsoft Visual Studio Code
* open the firmware in Microsoft Visual Studio Code

### Step 1: Initial upload via serial cable
* make sure your USB/FTDI is set to 5V
* insert your USB to FTDI onto J1 of the PCB
* press the switches in this order:
	- press RESET and hold
	- press PROG and hold
	- release PROG
	- release RESET
* this puts the device is programming mode (std ESP8266 procedure)
* if you have the buzzer installed, is should now be constantly beeping
* open the platformio.ini and configure the upload port and speed to use your USB/FTDI for example:
	- `upload_speed = 460800`
	- `upload_port = /dev/cu.usbserial-A50285BI`
* upload to the device
* during uploading the buzzer will make some different noises this is because its actually connect to an communication pin (normal)
* press the reset button and the device should boot, but it will take much longer then usual because the SPIFFS is not yet flashed

### Step 2: Upload the SPIFFS filesystem 
* seams to only work using `pio run -t uploadfs` (platformio "upload filesystem image" does not work)
* open terminal and execute the above command

### Step 3: switch to OTA
* reconfigure the platformio.ini file to now flash OTA:
	- `upload_protocol = espota`
	- `upload_port = 192.168.99.234` => **fill in the IP of YOUR device**
* try uploading the firmware again but this time OTA
* OTA uploading resets the device automatically so you don't have to press the RESET button (be patient!)
* now you can "Upload File Systems image" via Visual Studio Code / platformio
 

## MQTT

### Publishing


```
{room}
   └─── HixCO2
           └─── {deviceTag}
                     └─── device
                     │       └─── count
                     │       └─── room
                     │       └─── device_type
                     │       └─── device_version
                     │       └─── device_tag
                     │       └─── device_name
                     │       └─── wifi_ssid
                     │       └─── wifi_ip
                     │       
                     └─── status
                     │       └─── count
                     │       └─── wifi_rssi
                     │       └─── co2
                     │       └─── temperature
                     │
                     └─── influxdb
```
#### `influxdb` topic publishing contents
To make the import in the influxdb from Node-RED as easy as possible the `influxdb` topic publishes a json document that can immediatly imported using the `influxdb in` node.
They only thing required is to convert it to a json object via the `To json` node.
![Node-RED influxDB import](doc/nodered.png)

Following the `influxdb in` node the json is an array where:

- the first element contains the measurements
- the second element contains the tags
 

```
  [
   {
      "co2"           : 4564,
      "temperature"   : 45,
      "wifi_rssi"     : -45
   },
   {
      "device_type"   : "HixCO2",
      "device_version": "2.5.8",
      "device_tag"    : "at_door"
      "room"          : "office",
      "wifi_ssid"     : "public_wifi"
   }
  ]
```

### Subscribing

This device does not subscribe to any topics

## Configuration web interface

![Config web interface](doc/config.png)

* Once connectted to the WIFI a normal browser can be used to configure:
 - the MQTT details.
 - OTA = Over The Air updating of the firmware
 - LED enabled (if the device is installed e.g in bedroom the LED can be annoying)
 - OLED display enabled (same comment as for LED)
 - Selftest enabled (if on beeps, blinks and transmits IR Samsun Airco off at boot)
* Just go the the device's ip address with your browser
* Password is intitially set as `bornem1973`.
* At this moment the WIFI SSID and password are hardcoded in the src/secret.cpp file
