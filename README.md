# RP2040 Electric UI Example Project

Basic project demonstrating integration of the `electricui-embedded` library with Raspberry Pi Pico RP2040 microcontroller, using the `pico-sdk` C/C++ toolchain. Compatible with [the quickstart tutorial](https://electricui.com/docs/quick-start/).

We use the uart driver abstractions in preference to stdout, along with a non-blocking configurable LED blink demo.

In the main loop, any inbound bytes are passed to `eui_parse()` as they arrive. When a response is formed by eUI, it calls the `eui_write()` callback, which wraps the hal `uart_putc_raw()` call.

## Hardware Setup

- Developed against a [Raspberry Pi Pico](https://www.raspberrypi.org/products/raspberry-pi-pico/).
- Exposes eui access over `uart0` on GP0(TX)/GP1(RX) labelled pins. Connect a USB-UART (TTL) adapter to these pins (and ground).
- UART is configured to run at `115200` baud, 8N1 by default.
- The onboard green LED (GP25) is used as a blinker for the standard [`hello-electric` example](https://electricui.com/docs/quick-start/ui).

## Firmware Setup

This is a normal `pico-sdk` based example project and should work with your normal development toolchain, assuming `PICO_SDK_PATH` has a correct reference to your toolchain. 

## Dependencies

Relies on the `electricui-embedded` library, which can be [downloaded from GitHub](https://github.com/electricui/electricui-embedded), uncompressed, and placed in `/electricui-embedded/`.

The embedded library comes ready for CMake. Refer to the subdirectory and linker entries in the `CMakeLists.txt` in the root of this repo, or check out the [longform integration guide](https://electricui.com/docs/hardware/rp2040) for more detail.