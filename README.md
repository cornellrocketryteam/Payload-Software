# Payload Software
This project provides software support to the '24/'25 Air Sampler Payload. There are 3 main functions:
1. Controls 5 GPIO pins
2. Initiates sequence of commands based on BLE command from fill-station
3. Logs sequence of operation to SD Card

## How to Run

### Required Dependencies
* ```cmake```
* ```gcc-arm-embedded``` (for macOS)
* ```gcc-arm-none-eabi``` (for Linux)

_These dependencies can be install via [Homebrew](https://brew.sh/) on macOS or [apt](https://ubuntu.com/server/docs/package-management) on WSL._

### Setup
1. Install the required dependencies
2. Run ```git submodule update --init``` from within ```lib/```
3. Run ```git submodule update --init``` from within ```lib/pico-sdk/```

## Running
1. Create a top-level ```build/``` directory
2. Run ```cmake ..``` from within ```build/```
3. Run ```make```
4. Move ```PAYLOAD.uf2``` to a Pico in boot select mode
