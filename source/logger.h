/*
 * @brief Source file that contains the logger print functions
 *
 * Logger function header file.
 *
 * @authors Morgan Tilong, Xinyuan Zhoa
 * April 28th 2020
 */
#ifndef LOGGER_H_
#define LOGGER_H_

#include "stdlib.h"
#include "stdint.h"
#include "stdio.h"
#include "stdbool.h"

#include "MKL25Z4.h"
#include "fsl_debug_console.h"

extern uint8_t status;
extern unsigned int tenth;
#define tenthSec 286102UL;

//Logger level
typedef enum level
{
	Test,
	Debug,
	Normal,
}level;

//enable logger
void Log_enable();

//disable logger
void Log_disable();

//logger status
uint8_t Log_status();

//Logger output address for length of data
void Log_data(uint32_t* adress, size_t length);

//print string
void Log_string(char* s);

//print string with timestamp
void Log_messenger(enum level log_level, const char *function, char *message);

//print string and integer with timestamp
void Log_messenger_int(enum level log_level, const char *function, char *message, int number);

//integer format
void Log_integer_format(uint32_t *adress);

//.1 second delay
void Init_SysTick(void);

#endif /* LOGGER_H_*/
