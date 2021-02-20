cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(hello-blink-project)
pico_sdk_init()

add_executable( hello-blink
                main.c
)

pico_enable_stdio_usb(hello-blink 1)
pico_enable_stdio_uart(hello-blink 1)

pico_add_extra_outputs(hello-blink)

add_subdirectory(electricui-embedded)

target_link_libraries(hello-blink
        pico_stdlib
        pico_unique_id
        electricui
        )