#include "ds18b20.h"
#include <util/delay.h>

// Wire reset.
static uint8_t ds18b20_reset(void)
{
    // Set low signal for 480uS.
    DS18B20_LOW();
    DS18B20_OUTPUT_MODE();
    _delay_us(480);
    // Set high signal for 60uS.
    DS18B20_INPUT_MODE();
    _delay_us(60);
    // Read value on the line for 480uS
    // Read value: 0=OK, 1=no sensor found.
    uint8_t i = (DS18B20_PIN & (1 << DS18B20_DQ));
    _delay_us(420);

    return i;
}

// Send one bit over the wire.
static void ds18b20_write_bit(uint8_t bit)
{
    // Set low signal for 1uS.
    DS18B20_LOW();
    DS18B20_OUTPUT_MODE();
    _delay_us(1);
    // Set high signal if we want to send 1 otherwise keep it low.
    if (bit) {
        DS18B20_INPUT_MODE();
    }
    // Wait for 60uS and bring the line high back.
    _delay_us(60);
    DS18B20_INPUT_MODE();
}

// Read sigle bit from the wire.
static uint8_t ds18b20_read_bit(void)
{
    // Set low signal for 1uS.
    DS18B20_LOW();
    DS18B20_OUTPUT_MODE();
    _delay_us(1);
    // Bring it high for 14uS.
    DS18B20_INPUT_MODE();
    _delay_us(14);
    // Read wire status.
    uint8_t bit;
    if (DS18B20_PIN & (1<<DS18B20_DQ)) {
        bit = 1;
    } else {
        bit = 0;
    }
    // Wait for 45 uS.
    _delay_us(45);

    return bit;
}

// Read single byte from the wire.
static uint8_t ds18b20_read_byte(void)
{
    uint8_t i = 8;
    uint8_t n = 0;

    while (i--) {
        n >>= 1;
        n |= (ds18b20_read_bit() << 7);
    }

    return n;
}

// Write single byte from the wire.
static void ds18b20_write_byte(uint8_t byte)
{
    uint8_t i = 8;

    while (i--) {
        ds18b20_write_bit(byte & 1);
        byte >>= 1;
    }
}

// Read temperature value from sensor.
// Value is returned by given out parameter as a pair of integer
// and 1/1000 fractions.
void ds18b20_read_temp(int8_t *deg, uint16_t *fract)
{
    ds18b20_reset();
    ds18b20_write_byte(DS18B20_CMD_SKIPROM);
    ds18b20_write_byte(DS18B20_CMD_CONVERTTEMP);

    while (!ds18b20_read_bit());

    ds18b20_reset();
    ds18b20_write_byte(DS18B20_CMD_SKIPROM);
    ds18b20_write_byte(DS18B20_CMD_RSCRATCHPAD);

    uint8_t temp[2];
    temp[0] = ds18b20_read_byte();
    temp[1] = ds18b20_read_byte();
    ds18b20_reset();

    int8_t d;
    d = temp[0] >> 4;
    d |= (temp[1] & 0x7) << 4;
    *deg = d;

    uint16_t f;
    f = temp[0] & 0xF;
    f *= DS18B20_DECIMAL_STEPS_12BIT;
    f /= 100;
    *fract = f;
}
