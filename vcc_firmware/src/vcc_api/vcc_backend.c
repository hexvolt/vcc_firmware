/*
 * vcc_api.c
 *
 * Created: 15.01.2014 10:15:08
 *  Author: hexvolt
 */ 

#include <compiler.h>
#include <asf.h>
#include "vcc_backend.h"

void dac_set_voltage(USART_t *dac, port_pin_t dac_cs, uint16_t voltage)
{
	gpio_set_pin_low(dac_cs);
	usart_spi_transmit(dac, HI_UINT16(voltage));
	usart_spi_transmit(dac, LO_UINT16(voltage));
	gpio_set_pin_high(dac_cs);
}

void adc_get_voltages(USART_t *adc, bool u_range5v, bool i_range5v, uint32_t sram_address)
{
	register uint8_t x1, x2, x3, x4, y1, y2, y3, y4;
	register uint16_t val1, val2, t;

	// getting U1, U2
	ioport_set_pin_level(ADC_R0, u_range5v);			// setting ADC Range for U1, U2
	ioport_set_pin_level(ADC_ADDR, true);				// setting second channel (I1, I2) in queue for the next conversion
	ioport_set_pin_level(ADC_START, false);				// starting conversion of U1, U2
	ioport_set_pin_level(ADC_START, true);
	usart_init_spi(ADC, &ADC_SPI_OPTIONS);				// preparing the SPI of CONV1 bus for interaction with the ADC
	while (ioport_get_pin_level(ADC_BUSY));				// waiting while ADC ends the conversion
	ioport_set_pin_high(DAC1_CS);						
	ioport_set_pin_low(ADC_CS);							// getting the results
	x1 = usart_spi_transmit(ADC, 0);
	x2 = usart_spi_transmit(ADC, 0);
	x3 = usart_spi_transmit(ADC, 0);
	x4 = usart_spi_transmit(ADC, 0);
	ioport_set_pin_high(ADC_CS);
	
	// getting I1, I2
	ioport_set_pin_level(ADC_R0, i_range5v);			// setting ADC Range for I1, I2
	ioport_set_pin_level(ADC_ADDR, false);				// setting first channel (U1, U2) in queue for the next conversion
	ioport_set_pin_level(ADC_START, false);				// starting conversion of I1, I2
	ioport_set_pin_level(ADC_START, true);
	sram_start_write_sequence(sram_address);			// preparing to write measurement data to SRAM starting from the sram_address	
	while (ioport_get_pin_level(ADC_BUSY));				// waiting while ADC ends the conversion
	ioport_set_pin_low(ADC_CS);							// getting the results
	y1 = usart_spi_transmit(ADC, 0);
	y2 = usart_spi_transmit(ADC, 0);
	y3 = usart_spi_transmit(ADC, 0);
	y4 = usart_spi_transmit(ADC, 0);
	ioport_set_pin_high(ADC_CS);
	
	// processing results for U1, U2
	t = (x2 & 0b00000011) & 0x00FF;						// part from U1
	x2 = x2 & 0b11111100;
	x4 = x4 & 0b11110000;								// clearing unused bits
	val2 = ((x1 & 0x00FF) << 6) | (x2 >> 2);				// u2 without marker
	val1 = ((x3 & 0x00FF) << 4) | (x4 >> 4) | (t << 12);	// u1 without marker or with marker '00' (at MSB)
	val2 = val2 | (1 << 14);								// u2 with marker '01' (at MSB)
	sram_write_byte_from_sequence(HI_UINT16(val1));				// writing U1 to SRAM
	sram_write_byte_from_sequence(LO_UINT16(val1));				
	sram_write_byte_from_sequence(HI_UINT16(val2));				// writing U2 to SRAM
	sram_write_byte_from_sequence(LO_UINT16(val2));				
	
	// processing results for I1, I2
	t = (y2 & 0b00000011) & 0x00FF;						// part from I1
	y2 = y2 & 0b11111100;
	y4 = y4 & 0b11110000;								// clearing unused bits
	val2 = ((y1 & 0x00FF) << 6) | (y2 >> 2);				// I2 without marker
	val1 = ((y3 & 0x00FF) << 4) | (y4 >> 4) | (t << 12);	// I1 without marker or with marker '00' (at MSB)
	val2 = val2 | (1 << 14);								// I2 with marker '01' (at MSB)
	sram_write_byte_from_sequence(HI_UINT16(val1));				// writing I1 to SRAM
	sram_write_byte_from_sequence(LO_UINT16(val1));
	sram_write_byte_from_sequence(HI_UINT16(val2));				// writing I2 to SRAM
	sram_write_byte_from_sequence(LO_UINT16(val2));
	
	sram_end_write_sequence();							// !!! FUNCTION CAN BE OPTIMIZED BY ELIMINATING START-END SEQUENCE FUNCTIONS
	
	usart_init_spi(DAC1, &DAC_SPI_OPTIONS);				// restoring SPI settings from ADC to DAC
}

void sram_set_mode(uint8_t mode)
{
	gpio_set_pin_low(SRAM_CS);
	usart_spi_transmit(SRAM, SRAM_WRMR);				// sending command
	usart_spi_transmit(SRAM, mode);						// sending mode code
	gpio_set_pin_high(SRAM_CS);
}

void sram_write_byte(uint32_t address, uint8_t data) 
{
	gpio_set_pin_low(SRAM_CS);
	usart_spi_transmit(SRAM, SRAM_WRITE);				// sending command
	usart_spi_transmit(SRAM, HI_UINT32(address));		// sending address
	usart_spi_transmit(SRAM, MI_UINT32(address));
	usart_spi_transmit(SRAM, LO_UINT32(address));
	usart_spi_transmit(SRAM, LO_UINT16(data));			// sending data byte
	gpio_set_pin_high(SRAM_CS);
}

void sram_write_bytes_from_sequence(uint8_t data_array[], uint16_t data_length)
{
	uint8_t i;
	for (i = 0; i < data_length; i++)					// sending array of data
	{
		usart_spi_transmit(SRAM, data_array[i]);
	}
}

void sram_start_write_sequence(uint32_t address)
{
	gpio_set_pin_low(SRAM_CS);
	usart_spi_transmit(SRAM, SRAM_WRITE);				// sending command
	usart_spi_transmit(SRAM, HI_UINT32(address));		// sending address
	usart_spi_transmit(SRAM, MI_UINT32(address));
	usart_spi_transmit(SRAM, LO_UINT32(address));
}

void sram_write_sequence(uint32_t address, uint8_t data_array[], uint16_t data_length)
{
	uint16_t i;
	sram_start_write_sequence(address);
	for (i = 0; i < data_length; i++)					// sending array of data
	{
		usart_spi_transmit(SRAM, data_array[i]);		
	}
	sram_end_write_sequence();
}

uint8_t sram_read_byte(uint32_t address)
{
	uint8_t	data;
	gpio_set_pin_low(SRAM_CS);
	usart_spi_transmit(SRAM, SRAM_READ);				// sending command
	usart_spi_transmit(SRAM, HI_UINT32(address));		// sending address
	usart_spi_transmit(SRAM, MI_UINT32(address));
	usart_spi_transmit(SRAM, LO_UINT32(address));
	data = usart_spi_transmit(SRAM, 0);					// getting data byte
	gpio_set_pin_high(SRAM_CS);
	return data;
}

void sram_start_read_sequence(uint32_t address)
{
	gpio_set_pin_low(SRAM_CS);
	usart_spi_transmit(SRAM, SRAM_READ);				// sending command
	usart_spi_transmit(SRAM, HI_UINT32(address));		// sending address
	usart_spi_transmit(SRAM, MI_UINT32(address));
	usart_spi_transmit(SRAM, LO_UINT32(address));
}

void sram_read_sequence(uint32_t address, uint16_t data_length, uint8_t *data_array[])
{
	uint16_t i;
	sram_start_read_sequence(address);
	for (i = 0; i < data_length; i++)					// sending array of data
	{
		*data_array[i] = sram_read_byte_from_sequence();
	}
	sram_end_write_sequence();
}

void raspberry_send_string(const uint8_t message[], int msg_length)
{
	uint8_t i;
	for (i = 0; i < msg_length; i++) {
		usart_putchar(RASPBERRY_USART, message[i]);
	}
}

void raspberry_send_about(void)
{
	// to-do: need to read data from eeprom
 	const uint8_t about_str[] = "VCC digital unit v.3 \n\rStatus: OK \n\r";
	raspberry_send_string(about_str, sizeof(about_str)); 
}