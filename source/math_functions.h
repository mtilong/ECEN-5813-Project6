//includes

#ifndef MATH_FUNCTIONS_H_
#define MATH_FUNCTIONS_H_

#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"

#include "dma_driver.h"
#include "dac_adc_drivers.h"

#define PI 3.14

//LUT holding sine DAC outputs
extern uint16_t sinDAC[50];

//Math Calculations task counttaskYIELD();
extern uint16_t mathCounter;

//Executes the DSP function
extern TaskHandle_t dmaMath;


//sine wave generator
void sine_wave_LUT(void);

//DSP calculutions
void DSPMathOps(void *pvParameters);

void math_results(void);


//Output for DSP calculations


#endif /* SIN_WAVE_LUT_H_ */
