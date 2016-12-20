/*
 * vcc_api.h
 *
 * This unit implements a high-level functions for interaction with
 * the device via API
 *
 * Created: 17.01.2014 23:18:06
 *  Author: hexvolt
 */ 


#ifndef VCC_API_H_
#define VCC_API_H_

static const uint8_t OK_STR[] = "OK\r\n";
static const uint8_t UNKNOWN_STR[] = "Unknown command\r\n";

#define MAX_COMMAND_LENGTH 5

// Console API commands
#define CCMD_ABOUT				'A'
#define CCMD_LOAD				'L'
#define CCMD_ERASE				'E'
#define CCMD_GET_PROG			'P'
#define CCMD_GET_MEASURED_DATA	'D'
#define CCMD_START_MEASURE		'S'
#define CCMD_GET_LAST_MEASURE	'M'

typedef void (*TPTR)(void);

typedef enum 
{
	VCC_MODE_NORMAL_WAITING,
	VCC_MODE_LOADING_BYTECODE,
	VCC_MODE_MEASURING,
	VCC_MODE_RETURNING_DATA
} VCC_MODE_t;

extern VCC_MODE_t vcc_mode;
extern uint16_t bytecode_length;

void vcc_process_console_command(uint8_t console_command[], uint8_t cmd_length);
status_code_t process_api_command(uint8_t api_command[], uint8_t cmd_length);

//API functions
void set_z1_pow(void);
void set_z2_pow(void);
void set_z1_pol(void);
void set_z2_pol(void);
void dac1_out(void);
void dac2_out(void);
void pause(void);
void set_sensors_gain(void);
void measure(void);

#endif /* VCC_API_H_ */