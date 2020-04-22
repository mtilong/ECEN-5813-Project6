/*
 * @file adc_driver.c
 * @reference frdmkl25z_demo_apps_dac_adc
 *
 * This file has functions that
 * uses the DAC to generate a sine wave of 1V - 3V
 * of a period of 5 seconds
 *
 * @authors Morgan Tilong, Xinyuan Zhoa
 * @date April 28 2020
 */


#include "dac_adc_drivers.h"


//TaskHandle_t dac_task_handle;
//TaskHandle_t adc_task_handle;
uint8_t DACcounter = 0;

/*******************************************************************************
 * Code
 ******************************************************************************/
void DAC_Initialize(void)
{
    dac_config_t dacConfigStruct;

    /* Configure the DAC. */
    /*
     * dacConfigStruct.referenceVoltageSource = kDAC_ReferenceVoltageSourceVref2;
     * dacConfigStruct.enableLowPowerMode = false;
     */
    DAC_GetDefaultConfig(&dacConfigStruct);
    DAC_Init(DEMO_DAC_BASEADDR, &dacConfigStruct);
    DAC_Enable(DEMO_DAC_BASEADDR, true); /* Enable output. */
}

void DAC_task(void *pvParameters)
{
	const TickType_t xDelay = 100/portTICK_PERIOD_MS;
	TickType_t xLastProcessingTime;
	xLastProcessingTime = xTaskGetTickCount();

	while(1)
	{
		Log_messenger(Debug, __func__, "Updating DAC Value");
		taskYIELD();
		if(xSemaphoreTake(xMutex, 10) == pdTRUE)
		{

			LED_off();
			LED_cyclegreen_On();

			//For logger time keeping
			tenth++;

			//Output the LUT value to the DAC
			DAC_SetBufferValue(DEMO_DAC_BASEADDR, 0U, sinDAC[DACcounter++]);

			//Reset the DAC LUT index back to zero if limit reached
			if(DACcounter > 49)
			DACcounter = 0;

			LED_off();
			xSemaphoreGive(xMutex);
			taskYIELD();
		}
		vTaskDelayUntil( &xLastProcessingTime, xDelay );
	}
}



