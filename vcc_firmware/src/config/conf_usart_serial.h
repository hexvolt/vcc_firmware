/**
 * \file *********************************************************************
 *
 * \brief USART Serial configuration
 *
 * Copyright (c) 2011 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

#ifndef CONF_USART_SERIAL_H_INCLUDED
#define CONF_USART_SERIAL_H_INCLUDED

#define DAC1							&USARTE0
#define DAC2							&USARTC1
#define DAC_BAUDRATE					20000000	// 25 MHz max for AD5541
#define DAC_DATAORDER					false		// true or false (true = LSB first, false = MSB first)
#define DAC_SPIMODE						0			// 0, 1, 2 or 3 (see datasheet page 284)

#define SRAM							&USARTD1
#define SRAM_BAUDRATE					1000000		// 20 MHz max for 23LC1024
#define SRAM_DATAORDER					false		// true or false (true = LSB first, false = MSB first)
#define SRAM_SPIMODE					0			// 0, 1, 2 or 3 (see datasheet page 284)

#define ADC								&USARTE0
#define ADC_BAUDRATE					20000000	// 35 MHz max for AD7367
#define ADC_DATAORDER					false		// true or false (true = LSB first, false = MSB first)
#define ADC_SPIMODE						2			// 0, 1, 2 or 3 (see datasheet page 284)


#define RASPBERRY_USART                     &USARTD0
#define RASPBERRY_USART_BAUDRATE            57600	// 38400
#define RASPBERRY_USART_CHAR_LENGTH         USART_CHSIZE_8BIT_gc
#define RASPBERRY_USART_PARITY              USART_PMODE_DISABLED_gc
#define RASPBERRY_USART_STOP_BIT            false

extern usart_spi_options_t DAC_SPI_OPTIONS;
extern usart_spi_options_t ADC_SPI_OPTIONS;
extern usart_spi_options_t SRAM_SPI_OPTIONS;
extern usart_rs232_options_t RASP_USART_OPTIONS;


#endif /* CONF_USART_SERIAL_H_INCLUDED */
