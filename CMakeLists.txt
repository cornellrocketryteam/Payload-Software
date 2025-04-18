cmake_minimum_required(VERSION 3.22)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(PICO_BOARD pico_w CACHE STRING "Board type")
#set(CMAKE_EXPORT_COMPILE_COMMANDS ON) SHOULD I INCLUDE??

# Pull in SDK (must be before project)
include(lib/pico-sdk/pico_sdk_init.cmake)

project(PAYLOAD C CXX ASM) 

# Initialize the SDK
pico_sdk_init()
#add_definitions(-DRATS_VERBOSE) SHOULD I INCLUDE?

# Add compiler flags and definitions
add_compile_options(
    -Wall
    -Wno-format          
    -Wno-unused-function
)

# Add all executables
add_executable(${PROJECT_NAME}
    src/main.cpp
    src/pins.cpp
    src/mode.cpp
    src/sd.cpp
    src/hw_config.cpp
)

add_subdirectory(${PROJECT_SOURCE_DIR}/lib/no-OS-FatFS-SD-SDIO-SPI-RPi-Pico/src build)

include_directories(${PROJECT_SOURCE_DIR}/GAP_Advertisement)

# Add the standard include files to the build
target_include_directories(PAYLOAD PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/GAP_Advertisement
    ${CMAKE_CURRENT_LIST_DIR}/GATT_Service
)
# Pull in common dependencies
target_link_libraries(${PROJECT_NAME} 
    pico_stdlib
    pico_btstack_ble
    pico_btstack_cyw43
    pico_cyw43_arch_none
    no-OS-FatFS-SD-SDIO-SPI-RPi-Pico 
)

# Enable serial over USB
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

pico_btstack_make_gatt_header(PAYLOAD PRIVATE "${CMAKE_CURRENT_LIST_DIR}/GATT_Service/PAYLOAD.gatt")

# Create uf2 file
pico_add_uf2_output(${PROJECT_NAME}) #pico_add_extra_outputs(SRAD_RT) ?? which one? both?