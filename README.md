# MANGL - Mobile Application and Game Library
---

## Installation


## Dependencies linux

### Tools

    systemd-dev
    pkg-config


### Libraries

OpenAL

    libopenal-dev
    libalut-dev


SDL2
    libsdl2-dev




### SDL3 (optional)

Download and install from

    [https://wiki.libsdl.org/SDL3/FrontPage]

    libgles2-mesa-dev libudev-dev


Ubuntu 22.04+ (optional)

    libpipewire-0.3-dev libwayland-dev libdecor-0-dev

### Bluetooth Support

Install sdbus-cpp package

    libsdbus-c++-dev

Useful tools
    bluetoothctl    


## Installing emscripten 

Download emscripten

    git clone https://github.com/emscripten-core/emsdk.git

    cd emsdk

    ./emsdk install latest

    ./emsdk activate latest


## Project Configuration options

| Name                       | Type    | Description                         |
|----------------------------|---------|-------------------------------------|
| CONFIG_PROJECT_CONSOLE     | boolean | Build console application           |
| CONFIG_PROJECT_FORCE_UNIX  | boolean | Force Unix binary (WSL for example) |
| CONFIG_PROJECT_MULTIPLAYER | boolean | Compile with Multiplayer support    |
| CONFIG_PROJECT_BLUETOOTH   | boolean | Compile with Bluetooth support      |
| CONFIG_PROJECT_NO_CONFIG_H | boolean | Do not try include mangl_config.h   |
| CONFIG_PROJECT_UNIX_DAEMON | boolean | Build Unix Daemon                   |
| CONFIG_PROJECT_ADS         | boolean | Build with Ads support              |

