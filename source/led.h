/*
 * LED Initiate BLUE, RED, GREEN and OFF functions
 *
 * @authors Morgan Tilong, Xinyuan Zhoa
 * @ April 28th 2020
 */

#ifndef LED_H_
#define LED_H_

#include "stdio.h"
#include "board.h"

//KL24Z Library
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"

#include "logger.h"

//FreeRTOS
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"

// Macros
#define SCAN_OFFSET 544               // Offset for scan range
#define SCAN_DATA TSI0->DATA & 0xFFFF // Accessing the bits held in TSI0_DATA_TSICNT
#define MASK(x) (1UL << (x))

#define RED_LED_SHIFT   (18)	// on port B (PTB18)
#define GREEN_LED_SHIFT (19)	// on port B (PTB19)
#define BLUE_LED_SHIFT  (1)		// on port D (PTB1)

#define RED  (0)	// on port B (PTB18)
#define GREEN (1)	// on port B (PTB19)
#define BLUE  (2)  // on port D (PTB1)

extern uint16_t led_count;
extern SemaphoreHandle_t xMutex;
extern TaskHandle_t dma_led_task_handle;



//LED initiate functions
void KL25Z_RGB_Flasher(void);

void LED_off();

//Turn Green On
void LED_cyclegreen_On();

//Turn Red On
void LED_cyclered_On();

//Turn Blue On
void LED_cycleblue_On();

void LED_count();

void delay2(void);

#endif /*LED_H_*/
