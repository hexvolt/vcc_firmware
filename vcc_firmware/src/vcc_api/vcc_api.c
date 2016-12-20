/*
 * vcc_api.c
 *
 * Created: 17.01.2014 23:18:28
 *  Author: hexvolt
 */ 


#include <asf.h>
#include "vcc_backend.h"
#include "vcc_api.h"

uint8_t	api_param_h, api_param_l;
uint16_t bytecode_length;

const static TPTR api_hash_table[] PROGMEM =
{
	&dac1_out,			// 'a'
	&dac2_out,			// 'b'
	&set_z1_pol,		// 'c'
	&set_z2_pol,		// 'd'
	0,					// 'e'
	0,					// 'f'
	&set_sensors_gain,	// 'g'
	0,					// 'h'
	0,					// 'i'
	0,					// 'j'
	0,					// 'k'
	0,					// 'l'
	&measure,			// 'm'
	0,					// 'n'
	0,					// 'o'
	&pause,				// 'p'
	0,					// 'q'
	0,					// 'r'
	0,					// 's'
	0,					// 't'
	0,					// 'u'
	0,					// 'v'
	0,					// 'w'
	0,					// 'x'
	&set_z2_pow,		// 'y'
	&set_z1_pow,		// 'z'
	0,					// '{'
	0,					// '|'
	0					// '}'
};

void vcc_process_console_command(uint8_t console_command[], uint8_t cmd_length)
{
	uint8_t command;
	
	if (cmd_length > 0)
	{
		command = console_command[0];
	}
	else
	{
		command = 0;
	}
	
	switch (vcc_mode)
	{
		case VCC_MODE_MEASURING:				// if the module is measuring or returning data
		case VCC_MODE_RETURNING_DATA:			// then analyze '!' command and ignore the rest of API
			{
			
			}
			break;
		case VCC_MODE_LOADING_BYTECODE:			// if the module is loading bytecode, then transmit the command to memory
			{
				sram_write_bytes_from_sequence(console_command, cmd_length);			// transmitting command to memory
				bytecode_length += cmd_length;				// increasing length of bytecode in memory
				if (command == 0)							// if the end of bytecode received (command = 0)
				{
					vcc_mode = VCC_MODE_NORMAL_WAITING;		// then returning to normal mode
					sram_end_write_sequence();				// and end writing sequence
					sram_set_mode(SRAM_MODE_BYTE);			// returning to normal mode
				}
			}
			break;
		case VCC_MODE_NORMAL_WAITING:			// if the module is waiting for commands, then analyze it
			{
				switch (command)
				{
					case CCMD_ABOUT:
						{
							raspberry_send_about();
						}
						break;
					case CCMD_LOAD:
						{
							// preparing for loading bytecode
							vcc_mode = VCC_MODE_LOADING_BYTECODE;
							sram_set_mode(SRAM_MODE_SEQUENTIAL);		// setting sequential mode of SRAM
							sram_start_write_sequence(SRAM_PROGRAM_BASE_ADDR);		// preparing for writing bytecode to memory
							bytecode_length = 0;
						}
						break;
					case CCMD_ERASE:
						{
							// to-do: erasing SRAM, PC=0
						}
						break;
					case CCMD_GET_PROG:
						{
							// returning the program stored in SRAM
							uint16_t i, c;
							
							sram_set_mode(SRAM_MODE_SEQUENTIAL);		// setting sequential mode of SRAM
							sram_start_read_sequence(SRAM_PROGRAM_BASE_ADDR);		// preparing for reading bytecode from memory
							for (i = 0; i < bytecode_length; i++)
							{
								c = sram_read_byte_from_sequence();
								usart_putchar(RASPBERRY_USART, c);
							}
							sram_end_read_sequence();
							sram_set_mode(SRAM_MODE_BYTE);
						}
						break;
					case CCMD_GET_MEASURED_DATA:
						{
							// to-do: returning measured data from SRAM
						}
						break;
					case CCMD_GET_LAST_MEASURE:
						{
							// to-do: returning current ADC measure results
									usart_putchar(RASPBERRY_USART, sram_read_byte(SRAM_DATA_BASE_ADDR + 0));
									usart_putchar(RASPBERRY_USART, sram_read_byte(SRAM_DATA_BASE_ADDR + 1));
									usart_putchar(RASPBERRY_USART, sram_read_byte(SRAM_DATA_BASE_ADDR + 2));
									usart_putchar(RASPBERRY_USART, sram_read_byte(SRAM_DATA_BASE_ADDR + 3));
									usart_putchar(RASPBERRY_USART, sram_read_byte(SRAM_DATA_BASE_ADDR + 4));
									usart_putchar(RASPBERRY_USART, sram_read_byte(SRAM_DATA_BASE_ADDR + 5));
									usart_putchar(RASPBERRY_USART, sram_read_byte(SRAM_DATA_BASE_ADDR + 6));
									usart_putchar(RASPBERRY_USART, sram_read_byte(SRAM_DATA_BASE_ADDR + 7));
									usart_putchar(RASPBERRY_USART, '\r');
									usart_putchar(RASPBERRY_USART, '\n');
						}
						break;
					case CCMD_START_MEASURE:
						{
							// to-do: starting measuring via saved program, changing current mode to MEASURING
						}
						break;
					default:
						{
							if (process_api_command(console_command, cmd_length) == STATUS_OK)
							{
								raspberry_send_string(OK_STR, sizeof(OK_STR));
							}
							else
							{
								raspberry_send_string(UNKNOWN_STR, sizeof(UNKNOWN_STR));
							}
						}
				}
			}
	}
	
}

void set_z1_pow(void)
{
	switch (api_param_h)
	{
		case '0':		// turning off power
			{
				ioport_set_pin_level(U1_POW, false);
				ioport_set_pin_level(I1_POW, false);
			}
			break;
		case '1':		// turning on power of U1
			{
				ioport_set_pin_level(I1_POW, false);
				ioport_set_pin_level(U1_POW, true);
			}
			break;
		case '2':		// turning on power of I1
			{
				ioport_set_pin_level(U1_POW, false);
				ioport_set_pin_level(I1_POW, true);
			}
			break;
	}
}

void set_z2_pow(void)
{
	switch (api_param_h)
	{
		case '0':		// turning off power
		{
			ioport_set_pin_level(U2_POW, false);
			ioport_set_pin_level(I2_POW, false);
		}
		break;
		case '1':		// turning on power of U2
		{
			ioport_set_pin_level(I2_POW, false);
			ioport_set_pin_level(U2_POW, true);
		}
		break;
		case '2':		// turning on power of I2
		{
			ioport_set_pin_level(U2_POW, false);
			ioport_set_pin_level(I2_POW, true);
		}
		break;
	}
}

void set_z1_pol(void)
{
	ioport_set_pin_level(UI1_ZERO, (api_param_h == '0'));	// turning on or off GND input
	ioport_set_pin_level(UI1_POL, (api_param_h == '2'));	// switching the polarity of Z1 (1 - Plus, 2 - Minus)
}

void set_z2_pol(void)
{
	ioport_set_pin_level(UI2_ZERO, (api_param_h == '0'));	// turning on or off GND input
	ioport_set_pin_level(UI2_POL, (api_param_h == '2'));	// switching the polarity of Z1 (1 - Plus, 2 - Minus)
}

void dac1_out(void)
{
	uint16_t voltage;
	voltage = ((uint16_t)api_param_h << 8) | api_param_l;
	dac_set_voltage(DAC1, DAC1_CS, voltage);
}

void dac2_out(void)
{
	uint16_t voltage;
	voltage = ((uint16_t)api_param_h << 8) | api_param_l;
	dac_set_voltage(DAC2, DAC2_CS, voltage);
}

void pause(void)
{
	// to-do: pause
}

void set_sensors_gain(void)
{
	// setting gain of current sensor of Z1 channel
	ioport_set_pin_level(M1_G1, ((api_param_h == '2') || (api_param_h == '3')) );		// '0' Gain = 1; 
	ioport_set_pin_level(M1_G0, ((api_param_h == '1') || (api_param_h == '3')) );		// '1' Gain = 10;
	// setting gain of current sensor of Z2 channel										// '2' Gain = 100;
	ioport_set_pin_level(M2_G1, ((api_param_l == '2') || (api_param_l == '3')) );		// '3' Gain = 1000;
	ioport_set_pin_level(M2_G0, ((api_param_l == '1') || (api_param_l == '3')) );		
}

void measure(void)
{
	// measuring (ADC) and saving to SRAM
	adc_get_voltages(ADC, (api_param_h == '1'), (api_param_l == '1'), SRAM_DATA_BASE_ADDR);
}


status_code_t process_api_command(uint8_t api_command[], uint8_t cmd_length)
{
	uint8_t cmd_hash_index;
	status_code_t return_status;
	TPTR hash_function;
	
	return_status = ERR_BAD_ADDRESS;
	if (cmd_length > 1)							// if command has parameters then
	{
		api_param_h = api_command[1];			// getting first byte of parameter
		if (cmd_length > 2)
		{
			api_param_l = api_command[2];		// getting second byte of parameter
		}
	}
	if (cmd_length > 0)
	{
		if ((api_command[0] > 96) && (api_command[0] < 127))		// checking if the command is valid
		{
			cmd_hash_index = api_command[0] - 97;					// getting function address from hash-table 
			hash_function = (void*)nvm_flash_read_word((flash_addr_t)(api_hash_table + cmd_hash_index));
			if (hash_function > 0)
			{
				(hash_function)();				// Call function that service api_command
				return_status = STATUS_OK;
			}
		}
	}
	return return_status;
}
