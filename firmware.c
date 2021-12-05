// Teensy 2.0 + DS18B20 USB thermometer firmware.

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "ds18b20.h"
#include "usb_debug_only.h"

#define cpu_prescale(n) (CLKPR = 0x80, CLKPR = (n))

static inline void lod_on(void)
{
    PORTD &= ~(1 << 6);
}

static inline void led_off(void)
{
    PORTD |= (1 << 6);
}

static void usb_print(char *s)
{
    while (*s) {
        usb_debug_putchar(*s);
        s++;
    }
}

int main(void)
{
    // 16 MHz.
    cpu_prescale(0);
    led_off();
    usb_init();

    int8_t deg;
    uint16_t fract;
    char buf[10];

    for (;;) {
        ds18b20_read_temp(&deg, &fract);
        snprintf(buf, 10, "%d.%02u\n", deg, fract);
        usb_print(buf);
        _delay_ms(2000);
    }
}
