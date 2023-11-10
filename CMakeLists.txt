#  Copyright (c) Microsoft Corporation. All rights reserved.
#  Licensed under the MIT License.

cmake_minimum_required (VERSION 3.20)

project (neopixel-spi C)

azsphere_configure_tools(TOOLS_REVISION "23.05")
azsphere_configure_api(TARGET_API_SET "16")

# Create executable
add_executable (${PROJECT_NAME} 
    main.c
    pixels.c
    pixels.h
    effects/effects.h
    effects/blue_wave.c
    effects/rainbow.c
    )

target_link_libraries (${PROJECT_NAME} applibs pthread gcc_s c)

set(TARGET_HARDWARE "mt3620_rdb")
set(TARGET_DEFINITION "neopixel.json")
azsphere_target_hardware_definition(${PROJECT_NAME} TARGET_DIRECTORY "HardwareDefinition" TARGET_DEFINITION "${TARGET_DEFINITION}")

azsphere_target_add_image_package(${PROJECT_NAME})
