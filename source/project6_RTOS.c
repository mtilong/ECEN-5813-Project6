/*
 * @brief   Project 6
 *
 * Generate a sine wave signal on Kl25Z DAC, capture signal in ADC.
 * Transfer collected data buffer via DMA to another data buffer, and perform
 * basic DSP analysis of the signal.
 *
 * @date April 28th 2020
 * @author Morgan Tilong, Xinyuan Zhoa
 */

#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <math_functions.h>
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "pin_mux.h"

//headers...
#include "logger.h"
#include "led.h"
#include "dac_adc_drivers.h"
#include "dma_driver.h"
#include "dma_driver.h"

//FreeRTOS Libraries...
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"
/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 *
 */

int main(void)
{
	xMutex = xSemaphoreCreateMutex();
    /* Init board hardware. */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    SystemCoreClockUpdate();

    Log_enable();
    KL25Z_RGB_Flasher();
    EnableIRQ(ADC0_IRQn);

	PRINTF("\n");

	//log
	Log_messenger(Debug, __func__, "Generating LUT for sine wave");
	Log_messenger(Normal, __func__, "Generating LUT for sine wave");
	//Generate the sine LUT
	sine_wave_LUT();

	Log_messenger(Debug, __func__, "Initializing the DAC");
	DAC_Initialize();//Init the DAC


	Log_messenger(Debug, __func__, "Initializing the ADC");
	ADC_Initialize();//Init the ADC

	Log_messenger(Debug, __func__, "Initializing the DMA");
	DMA_init();

	//Create the task that runs the ADC
	xTaskCreate(ADC_task, "ADC_task", configMINIMAL_STACK_SIZE + 128, NULL, tskIDLE_PRIORITY + 1, adc_task_handle);
	//Create the task DAC
	xTaskCreate(DAC_task, "DAC_task", configMINIMAL_STACK_SIZE + 128, NULL, tskIDLE_PRIORITY + 1, dac_task_handle);

	Log_messenger(Debug, __func__, "Updating signals from DAC every 100mS");
	Log_messenger(Normal, __func__, "Updating signals from DAC every 100mS");
	Log_messenger(Debug, __func__, "Reading signals from ADC every 100mS");
	Log_messenger(Normal, __func__, "Reading signals from ADC every 100mS");

	//create the queue that holds the ADC values
	adcQueue = xQueueCreate(64, sizeof(uint32_t));
	//Check if the queue is NULL
	if(adcQueue == NULL){
		printf("Queue creation failed\n");
	} else {
		printf("Queue creation passed\n");
	}

	//start scheduler
	vTaskStartScheduler();
	for (;;)
		;
	return 0 ;
}



