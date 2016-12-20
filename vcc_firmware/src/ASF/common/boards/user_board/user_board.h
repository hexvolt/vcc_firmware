/**
 * \file
 *
 * \brief User board definition template
 *
 */

 /* This file is intended to contain definitions and configuration details for
 * features and devices that are available on the board, e.g., frequency and
 * startup time for an external crystal, external memory devices, LED and USART
 * pins.
 */

#ifndef USER_BOARD_H
#define USER_BOARD_H

#include <conf_board.h>

// External oscillator settings.
// Uncomment and set correct values if external oscillator is used.

// External oscillator frequency
//#define BOARD_XOSC_HZ          8000000

// External oscillator type.
//!< External clock signal
//#define BOARD_XOSC_TYPE        XOSC_TYPE_EXTERNAL
//!< 32.768 kHz resonator on TOSC
//#define BOARD_XOSC_TYPE        XOSC_TYPE_32KHZ
//!< 0.4 to 16 MHz resonator on XTALS
//#define BOARD_XOSC_TYPE        XOSC_TYPE_XTAL

// External oscillator startup time
//#define BOARD_XOSC_STARTUP_US  500000

//Raspberry Pi pins connections
#define RASP_TX			IOPORT_CREATE_PIN(PORTD, 2)
#define RASP_RX			IOPORT_CREATE_PIN(PORTD, 3)

//Converters pins connections
#define CONV1_SI		IOPORT_CREATE_PIN(PORTE, 3)
#define CONV1_SO		IOPORT_CREATE_PIN(PORTE, 2)
#define CONV1_SCK		IOPORT_CREATE_PIN(PORTE, 1)
#define CONV2_SI		IOPORT_CREATE_PIN(PORTC, 7)
#define CONV2_SO		IOPORT_CREATE_PIN(PORTC, 6)
#define CONV2_SCK		IOPORT_CREATE_PIN(PORTC, 5)

//SRAM pins connections
#define SRAM_SI			IOPORT_CREATE_PIN(PORTD, 7)
#define SRAM_SO			IOPORT_CREATE_PIN(PORTD, 6)
#define SRAM_SCK		IOPORT_CREATE_PIN(PORTD, 5)

//SPI Chip Select pins connections
#define ADC_CS			IOPORT_CREATE_PIN(PORTR, 0)
#define DAC1_CS			IOPORT_CREATE_PIN(PORTR, 1)
#define DAC2_CS			IOPORT_CREATE_PIN(PORTC, 4)
#define SRAM_CS			IOPORT_CREATE_PIN(PORTD, 0)

//ADC pins connections
#define ADC_START		IOPORT_CREATE_PIN(PORTE, 0)
#define ADC_BUSY		IOPORT_CREATE_PIN(PORTD, 4)
#define ADC_ADDR		IOPORT_CREATE_PIN(PORTC, 3)
#define ADC_R0			IOPORT_CREATE_PIN(PORTC, 2)
#define ADC_R1			IOPORT_CREATE_PIN(PORTD, 1)

//ANALOG UNIT CONNECTIONS

//Sources power pins connections
#define U1_POW			IOPORT_CREATE_PIN(PORTA, 2)
#define I1_POW			IOPORT_CREATE_PIN(PORTA, 5)
#define U2_POW			IOPORT_CREATE_PIN(PORTB, 2)
#define I2_POW			IOPORT_CREATE_PIN(PORTB, 0)

//Sources polarity pins connections
#define UI1_POL			IOPORT_CREATE_PIN(PORTA, 1)
#define UI1_ZERO		IOPORT_CREATE_PIN(PORTA, 0)
#define UI2_POL			IOPORT_CREATE_PIN(PORTB, 3)
#define UI2_ZERO		IOPORT_CREATE_PIN(PORTB, 1)

//Current Sensors Gain pins connections
#define M1_G0			IOPORT_CREATE_PIN(PORTA, 3)
#define M1_G1			IOPORT_CREATE_PIN(PORTA, 4)
#define M2_G0			IOPORT_CREATE_PIN(PORTA, 7)
#define M2_G1			IOPORT_CREATE_PIN(PORTA, 6)

//Analog Unit EEPROM I2C pins
#define EEPROM_SCL		IOPORT_CREATE_PIN(PORTC, 1)
#define EEPROM_SDA		IOPORT_CREATE_PIN(PORTC, 0)

//Other pins connections
#define LED				ADC_CS


#endif // USER_BOARD_H
