// Basic RP2040 UART Demo with electricui-embedded
// Follows hello-blink for compatibility with UI tutorials

#include <stdio.h>
#include "pico/stdlib.h"

#include "hardware/gpio.h"
#include "hardware/uart.h"
#include "hardware/timer.h"

#include "pico/binary_info.h"
#include "pico/unique_id.h"

#include "electricui.h"


const uint8_t LED_PIN = 25;

// Simple variables to modify the LED behaviour
uint8_t   blink_enable = 1; // if the blinker should be running
uint8_t   led_state  = 0;   // track if the LED is illuminated
uint16_t  glow_time  = 200; // in milliseconds
uint32_t  led_timer  = 0;   // track when the light turned on or off

char nickname[] = "Pico Demo";

void eui_write( uint8_t *data, uint16_t size );

eui_interface_t serial_comms = EUI_INTERFACE( &eui_write );

eui_message_t tracked_variables[] =
{
        EUI_UINT8(  "led_blink",  blink_enable ),
        EUI_UINT8(  "led_state",  led_state ),
        EUI_UINT16( "lit_time",   glow_time ),
        EUI_CHAR_ARRAY_RO( "name", nickname ),
};

// This function is called by eUI to send data to the PC
void eui_write( uint8_t *data, uint16_t size )
{
    for( uint16_t i = 0; i < size; i++ )
    {
        uart_putc_raw( uart0, data[i] );
    }
}


int main()
{
    bi_decl( bi_program_description("Electric UI Serial Demo") );
    bi_decl( bi_1pin_with_name(LED_PIN,"On-board LED") );

    stdio_init_all();

    // Configure the onboard LED and UART0 on pins GP0 and GP1
    gpio_init( LED_PIN );
    gpio_set_dir( LED_PIN, GPIO_OUT );

    uart_init( uart0, 115200 );
    gpio_set_function( 0, GPIO_FUNC_UART );
    gpio_set_function( 1, GPIO_FUNC_UART );


    // Setup eUI's interface and tracked variables
    eui_setup_interface( &serial_comms );
    EUI_TRACK( tracked_variables );

    // Get the UUID from the micro's flash chip, pass it to eUI
    pico_unique_board_id_t board_id;
    pico_get_unique_board_id(&board_id);
    eui_setup_identifier((char *)&board_id.id, PICO_UNIQUE_BOARD_ID_SIZE_BYTES );

    led_timer = us_to_ms( time_us_32() );


    while( true )
    {
        while( uart_is_readable( uart0 ) )
        {
            eui_parse( uart_getc( uart0 ), &serial_comms );
        }

        if( blink_enable )
        {
            uint32_t time_ms = us_to_ms( time_us_32() );

            // Check if the LED has been on for the configured duration
            if( time_ms - led_timer >= glow_time )
            {
                led_state = !led_state;     //invert led state
                led_timer = time_ms;
            }
        }

        gpio_put( LED_PIN, led_state );
    }

}