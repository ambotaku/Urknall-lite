cmake_minimum_required(VERSION 3.12)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(urknall 
    LANGUAGES C CXX ASM 
    VERSION 1.0 
    DESCRIPTION "Urknall - Conway's game of life")


set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(urknall
    urknall.cpp
    GFX.cpp
    Habitat.cpp
    SSD1306.cpp
 )

target_link_libraries(${CMAKE_PROJECT_NAME}
  pico_stdlib
  hardware_i2c
)

pico_enable_stdio_uart(${CMAKE_PROJECT_NAME} 1)
pico_enable_stdio_usb(${CMAKE_PROJECT_NAME} 0)
pico_add_extra_outputs(${CMAKE_PROJECT_NAME})
