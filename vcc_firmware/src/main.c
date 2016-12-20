/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
#include <asf.h>
#include "vcc_api/vcc_backend.h"
#include "vcc_api/vcc_api.h"

VCC_MODE_t vcc_mode = VCC_MODE_NORMAL_WAITING;				// Current device mode (global extern)

ISR(USARTD0_RXC_vect)										// Interrupt (Received a byte from Raspberry)
{
	static uint8_t command_buf[MAX_COMMAND_LENGTH];							
	static uint8_t i=0;
	uint8_t c;
	
	c = usart_get(RASPBERRY_USART);							// Read a character from Raspberry
	if (i < MAX_COMMAND_LENGTH)
	{
		command_buf[i] = c;									// Add the character to command_buf
		i++;												// Increase length of command
	}
	if (c == '\r')
	{
		vcc_process_console_command(command_buf, i-1);		// Try to execute a command after Enter
		i = 0;
	}
}

int main (void)
{
	sysclk_init();											// Initializing sys_clock
	ioport_init();											// Initializing GPIO
	board_init();											// Initializing uC pins for onboard devices
	pmic_init();											// Initializing interrupt controller
	
	// Initializing serial interfaces for onboard devices
	usart_init_spi(DAC1, &DAC_SPI_OPTIONS);
	usart_init_spi(DAC2, &DAC_SPI_OPTIONS);
	usart_init_spi(SRAM, &SRAM_SPI_OPTIONS);
	usart_init_rs232(RASPBERRY_USART, &RASP_USART_OPTIONS);
	
	usart_set_rx_interrupt_level(RASPBERRY_USART, USART_INT_LVL_LO);
	
	// Code
	dac_set_voltage(DAC1, DAC1_CS, 0x7FFF);
	dac_set_voltage(DAC2, DAC2_CS, 0xFFFF);
		
	cpu_irq_enable();
	
	uint8_t mas[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
	sram_set_mode(SRAM_MODE_SEQUENTIAL);
	sram_write_sequence(1, mas, 7);
	
	sram_set_mode(SRAM_MODE_SEQUENTIAL);	
 	usart_putchar(RASPBERRY_USART, sram_read_byte(0x00000001));
 	usart_putchar(RASPBERRY_USART, sram_read_byte(0x00000002));
 	usart_putchar(RASPBERRY_USART, sram_read_byte(0x00000003));
 	usart_putchar(RASPBERRY_USART, sram_read_byte(0x00000004));
 	usart_putchar(RASPBERRY_USART, sram_read_byte(0x00000005));
 	usart_putchar(RASPBERRY_USART, sram_read_byte(0x00000006));
 	usart_putchar(RASPBERRY_USART, sram_read_byte(0x00000007));
	
	while (true) {
		
	}
}
