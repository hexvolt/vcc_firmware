/**
 * \file
 *
 * \brief User board initialization template
 *
 */

#include <asf.h>
#include <board.h>
#include <conf_board.h>

// global variables to store serial interfaces settings
usart_spi_options_t DAC_SPI_OPTIONS = {
	.baudrate = DAC_BAUDRATE,
	.data_order = DAC_DATAORDER,
	.spimode = DAC_SPIMODE
};

usart_spi_options_t ADC_SPI_OPTIONS = {
	.baudrate = ADC_BAUDRATE,
	.data_order = ADC_DATAORDER,
	.spimode = ADC_SPIMODE
};

usart_spi_options_t SRAM_SPI_OPTIONS = {
	.baudrate = SRAM_BAUDRATE,
	.data_order = SRAM_DATAORDER,
	.spimode = SRAM_SPIMODE
};

usart_rs232_options_t RASP_USART_OPTIONS = {
	.baudrate = RASPBERRY_USART_BAUDRATE,
	.charlength = RASPBERRY_USART_CHAR_LENGTH,
	.paritytype = RASPBERRY_USART_PARITY,
	.stopbits = RASPBERRY_USART_STOP_BIT
};

void board_init(void)
{
	// Initialization code for I/O pins of Digital Unit
	ioport_configure_pin(RASP_TX, IOPORT_DIR_INPUT);
	ioport_configure_pin(RASP_RX, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);

	//Converters pins initialization
	ioport_configure_pin(CONV1_SI, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);
	ioport_configure_pin(CONV1_SO, IOPORT_DIR_INPUT);
	ioport_configure_pin(CONV1_SCK, IOPORT_DIR_OUTPUT | IOPORT_INIT_LOW);			// for ADC need to be high before transfer starts
	ioport_configure_pin(CONV2_SI, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);
	ioport_configure_pin(CONV2_SO, IOPORT_DIR_INPUT);
	ioport_configure_pin(CONV2_SCK, IOPORT_DIR_OUTPUT | IOPORT_INIT_LOW);

	//SRAM pins initialization
	ioport_configure_pin(SRAM_SI, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);
	ioport_configure_pin(SRAM_SO, IOPORT_DIR_INPUT);
	ioport_configure_pin(SRAM_SCK, IOPORT_DIR_OUTPUT | IOPORT_INIT_LOW);

	//SPI Chip Select pins initialization
	ioport_configure_pin(ADC_CS, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);
	ioport_configure_pin(DAC1_CS, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);
	ioport_configure_pin(DAC2_CS, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);
	ioport_configure_pin(SRAM_CS, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);

	//ADC pins initialization
	ioport_configure_pin(ADC_START, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);
	ioport_configure_pin(ADC_BUSY, IOPORT_DIR_INPUT);
	ioport_configure_pin(ADC_ADDR, IOPORT_DIR_OUTPUT | IOPORT_INIT_LOW);	// Initial measurement target is Va1 & Vb1 (voltages)
	ioport_configure_pin(ADC_R0, IOPORT_DIR_OUTPUT | IOPORT_INIT_LOW);		// Initial Range is +-10V
	ioport_configure_pin(ADC_R1, IOPORT_DIR_OUTPUT | IOPORT_INIT_LOW);		

	// ANALOG UNIT initialization

	// Sources power pins initialization
	ioport_configure_pin(U1_POW, IOPORT_DIR_OUTPUT | IOPORT_INIT_LOW);		// all
	ioport_configure_pin(I1_POW, IOPORT_DIR_OUTPUT | IOPORT_INIT_LOW);		// signal sources
	ioport_configure_pin(U2_POW, IOPORT_DIR_OUTPUT | IOPORT_INIT_LOW);		// are
	ioport_configure_pin(I2_POW, IOPORT_DIR_OUTPUT | IOPORT_INIT_LOW);		// off

	// Sources polarity pins initialization
	ioport_configure_pin(UI1_POL, IOPORT_DIR_OUTPUT | IOPORT_INIT_LOW);		// all
	ioport_configure_pin(UI1_ZERO, IOPORT_DIR_OUTPUT | IOPORT_INIT_LOW);	// analog switches
	ioport_configure_pin(UI2_POL, IOPORT_DIR_OUTPUT | IOPORT_INIT_LOW);		// are in Z-state, so
	ioport_configure_pin(UI2_ZERO, IOPORT_DIR_OUTPUT | IOPORT_INIT_LOW);	// driving signal equals zero

	// Current Sensors Gain pins initialization
	ioport_configure_pin(M1_G0, IOPORT_DIR_OUTPUT | IOPORT_INIT_LOW);		// default 
	ioport_configure_pin(M1_G1, IOPORT_DIR_OUTPUT | IOPORT_INIT_LOW);		// gains
	ioport_configure_pin(M2_G0, IOPORT_DIR_OUTPUT | IOPORT_INIT_LOW);		// are
	ioport_configure_pin(M2_G1, IOPORT_DIR_OUTPUT | IOPORT_INIT_LOW);		// 0

	// Analog Unit EEPROM I2C pins initialization
	ioport_configure_pin(EEPROM_SCL, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);	// SCL
	ioport_configure_pin(EEPROM_SDA, IOPORT_DIR_INPUT | IOPORT_PULL_UP);	// SDA
	
}
