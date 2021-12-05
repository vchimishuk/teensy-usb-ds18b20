// Based on lamazavr's code published
// at https://blablacode.ru/mikrokontrollery/442

#ifndef DS18B20_H
#define DS18B20_H

#include <stdint.h>
#include <avr/io.h>

#define DS18B20_PORT PORTB
#define DS18B20_DDR DDRB
#define DS18B20_PIN PINB
#define DS18B20_DQ PB7
#define DS18B20_INPUT_MODE() (DS18B20_DDR &= ~(1 << DS18B20_DQ))
#define DS18B20_OUTPUT_MODE() DS18B20_DDR |= (1 << DS18B20_DQ)
#define DS18B20_LOW() (DS18B20_PORT &= ~(1 << DS18B20_DQ))
#define DS18B20_HIGH() (DS18B20_PORT |= (1 << DS18B20_DQ))

// DS18B20 sensor commands.
#define DS18B20_CMD_CONVERTTEMP 0x44
#define DS18B20_CMD_RSCRATCHPAD 0xBE
#define DS18B20_CMD_WSCRATCHPAD 0x4E
#define DS18B20_CMD_CPYSCRATCHPAD 0x48
#define DS18B20_CMD_RECEEPROM 0xB8
#define DS18B20_CMD_RPWRSUPPLY 0xB4
#define DS18B20_CMD_SEARCHROM 0xF0
#define DS18B20_CMD_READROM 0x33
#define DS18B20_CMD_MATCHROM 0x55
#define DS18B20_CMD_SKIPROM 0xCC
#define DS18B20_CMD_ALARMSEARCH 0xEC
#define DS18B20_DECIMAL_STEPS_12BIT 625

void ds18b20_read_temp(int8_t *deg, uint16_t *fract);

#endif // DS18B20_H
