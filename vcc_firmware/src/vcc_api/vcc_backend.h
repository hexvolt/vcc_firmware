/*
 * vcc_api.h
 *
 * This unit implements low-level functions and constants for interaction 
 * with all of digital ICs of device like SRAM, DACs, ADCs etc.
 * 
 * Created: 15.01.2014 10:12:28
 *  Author: hexvolt
 */ 


#ifndef VCC_BACKEND_H_
#define VCC_BACKEND_H_

#define HI_UINT16(a) (((a) >> 8) & 0xFF)
#define LO_UINT16(a) ((a) & 0xFF)

#define HI_UINT32(a) (((a) >> 16) & 0xFF)
#define MI_UINT32(a) (((a) >> 8) & 0xFF)
#define LO_UINT32(a) ((a) & 0xFF)

// SRAM commands (23LC1024)
#define SRAM_READ		0x03	// Read data from memory array beginning at selected address
#define SRAM_WRITE		0x02	// Write data to memory array beginning at selected address
#define SRAM_EDIO		0x3B	// Enter Dual I/O access
#define SRAM_EQIO		0x38	// Enter Quad I/O access
#define SRAM_RSTIO		0xFF	// Reset Dual and Quad I/O access
#define SRAM_RDMR		0x05	// Read Mode Register
#define SRAM_WRMR		0x01	// Write Mode Register

#define SRAM_MODE_BYTE	0x00	// Byte mode of SRAM
#define SRAM_MODE_SEQUENTIAL 0x40	// Sequential mode of SRAM
#define SRAM_MODE_PAGE	0x80	// Page mode of SRAM

#define SRAM_PROGRAM_BASE_ADDR 0x00000
#define SRAM_DATA_BASE_ADDR	0x02000	// 8K - Base address of measurement data in SRAM (Full range 0x00000 - 0x1FFFF)

/**
 * \brief Sets the output voltage of specified DAC.
 *
 * \param dac The USART module assigned with the DAC via SPI bus.
 * \param dac_cs The Chip Select pin assigned with DAC.
 * \param voltage The voltage level in range 0x0000 - 0xFFFF.
  */
void dac_set_voltage(USART_t *dac, port_pin_t dac_cs, uint16_t voltage);

/**
 * \brief Measures the output voltages of sensors U1, U2, I1, I2 and stores the results into SRAM.
 *
 * \param adc The USART module assigned with the ADC via SPI bus.
 * \param u_range5v Reference voltage of ADC for U1 U2 measurement (+-5V or +-10V).
 * \param i_range5v Reference voltage of ADC for I1 I2 measurement (+-5V or +-10V).
 * \param sram_address The base address of SRAM to write the results.
  */
void adc_get_voltages(USART_t *adc, bool u_range5v, bool i_range5v, uint32_t sram_address);

/**
 * \brief Sets the mode register of SRAM chip
 *
 * \param mode The mode of data read/write.
  */
void sram_set_mode(uint8_t mode);

/**
 * \brief Writes byte to external SRAM memory chip. Need to be in BYTE MODE (which is default).
 *
 * \param address The target address of memory cell to write data (0x00000000 - 0x0001FFFF for 23LC1024).
 * \param data The data byte.
  */
void sram_write_byte(uint32_t address, uint8_t data);

/**
 * \brief Prepare SRAM chip for writing sequence of bytes. Need to be in SEQUENTIAL MODE.
 * Sends the command and address to SRAM.
 * 
 * \address The base target address of memory cell to start data write (0x00000000 - 0x0001FFFF for 23LC1024).
  */
void sram_start_write_sequence(uint32_t address);

/**
 * \brief Writes byte from sequence to external SRAM memory chip. Need to be in SEQUENTIAL MODE.
 * Call after sram_start_write_sequence() function
 *
 * \a_byte The data byte to write.
  */
#define sram_write_byte_from_sequence(a_byte) \
	usart_spi_transmit(SRAM, a_byte)

/**
 * \brief Writes array of bytes from sequence to external SRAM memory chip. Need to be in SEQUENTIAL MODE.
 * Call after sram_start_write_sequence() function
 *
 * \data_array[] The array of data bytes to write.
 * \data_length Number of bytes.
  */
void sram_write_bytes_from_sequence(uint8_t data_array[], uint16_t data_length);

/**
 * \brief Ends the interaction with SRAM memory chip. Need to be in SEQUENTIAL MODE.
 *
  */
#define sram_end_write_sequence() \
	gpio_set_pin_high(SRAM_CS);

/**
 * \brief Writes a sequence of data to external SRAM memory chip. Need to be in SEQUENTIAL MODE.
 *
 * \param address The base target address of memory cell to start data write (0x00000000 - 0x0001FFFF for 23LC1024).
 * \param data_array[] The array of data to be send to.
 * \param data_length The number of bytes to be send to.
  */
void sram_write_sequence(uint32_t address, uint8_t data_array[], uint16_t data_length);

/**
 * \brief Reads byte from external SRAM memory chip.
 *
 * \param address The address of memory cell to read data from (0x00000000 - 0x0001FFFF for 23LC1024).
  * \return The data byte
  */
uint8_t sram_read_byte(uint32_t address);

/**
 * \brief Prepare SRAM chip for reading sequence of bytes. Need to be in SEQUENTIAL MODE.
 * Sends the command and address to SRAM.
 * 
 * \address The base target address of memory cell to start data write (0x00000000 - 0x0001FFFF for 23LC1024).
  */
void sram_start_read_sequence(uint32_t address);

/**
 * \brief Reads the next data byte from external SRAM memory chip. Need to be in SEQUENTIAL MODE.
 * Call after sram_start_read_sequence() function
 *
 * \return The data byte
  */
#define sram_read_byte_from_sequence() \
	usart_spi_transmit(SRAM, 0);

/**
 * \brief Ends the interaction with SRAM memory chip. Need to be in SEQUENTIAL MODE.
 *
  */
#define sram_end_read_sequence() \
	gpio_set_pin_high(SRAM_CS);

/**
 * \brief Prepare SRAM chip for reading sequence of bytes. Need to be in SEQUENTIAL MODE.
 * Sends the command and address to SRAM.
 * 
 * \address The base target address of memory cell to read data (0x00000000 - 0x0001FFFF for 23LC1024).
 * \data_length The number of bytes to read from SRAM.
 * \*data_array The buffer to receive read data.
  */
void sram_read_sequence(uint32_t address, uint16_t data_length, uint8_t *data_array[]);

void raspberry_send_string(const uint8_t message[], int msg_length);

void raspberry_send_about(void);


#endif /* VCC_BACKEND_H */