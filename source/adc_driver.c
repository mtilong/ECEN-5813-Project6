/*
 * @file adc_driver.c
 * @reference frdmkl25z_demo_apps_dac_adc
 *
 * This file has functions that
 * uses the ADC to sample a sine wave of 1V - 3V
 * of a period of 5 seconds
 *
 * @authors Morgan Tilong, Xinyuan Zhoa
 * @date April 28 2020
 */

#include "dac_adc_drivers.h"



volatile bool g_Adc16ConversionDoneFlag = false;
volatile uint32_t g_Adc16ConversionValue = 0;


adc16_channel_config_t g_adc16ChannelConfigStruct;

/* ADC input queue handle */
QueueHandle_t adcQueue = NULL;


//TaskHandle_t dac_task_handle;
//TaskHandle_t adc_task_handle;

void ADC_Initialize(void)
{
	adc16_config_t adc16ConfigStruct;

	/* Configure the ADC16. */
	    /*
	     * adc16ConfigStruct.referenceVoltageSource = kADC16_ReferenceVoltageSourceVref;
	     * adc16ConfigStruct.clockSource = kADC16_ClockSourceAsynchronousClock;
	     * adc16ConfigStruct.enableAsynchronousClock = true;
	     * adc16ConfigStruct.clockDivider = kADC16_ClockDivider8;
	     * adc16ConfigStruct.resolution = kADC16_ResolutionSE12Bit;
	     * adc16ConfigStruct.longSampleMode = kADC16_LongSampleDisabled;
	     * adc16ConfigStruct.enableHighSpeed = false;
	     * adc16ConfigStruct.enableLowPower = false;
	     * adc16ConfigStruct.enableContinuousConversion = false;
	     */
	    ADC16_GetDefaultConfig(&adc16ConfigStruct);
	#if defined(BOARD_ADC_USE_ALT_VREF)
	    adc16ConfigStruct.referenceVoltageSource = kADC16_ReferenceVoltageSourceValt;
	#endif
	    ADC16_Init(DEMO_ADC16_BASEADDR, &adc16ConfigStruct);

	    /* Make sure the software trigger is used. */
	    ADC16_EnableHardwareTrigger(DEMO_ADC16_BASEADDR, false);
	#if defined(FSL_FEATURE_ADC16_HAS_CALIBRATION) && FSL_FEATURE_ADC16_HAS_CALIBRATION
	    if (kStatus_Success == ADC16_DoAutoCalibration(DEMO_ADC16_BASEADDR))
	    {
	        PRINTF("\r\nADC16_DoAutoCalibration() Done.\n");
	    }
	    else
	    {
	        PRINTF("ADC16_DoAutoCalibration() Failed.\r\n");
	    }
	#endif /* FSL_FEATURE_ADC16_HAS_CALIBRATION */

	    /* Prepare ADC channel setting */
	    g_adc16ChannelConfigStruct.channelNumber = DEMO_ADC16_USER_CHANNEL;
	    g_adc16ChannelConfigStruct.enableInterruptOnConversionCompleted = true;

	#if defined(FSL_FEATURE_ADC16_HAS_DIFF_MODE) && FSL_FEATURE_ADC16_HAS_DIFF_MODE
	    g_adc16ChannelConfigStruct.enableDifferentialConversion = false;
	#endif /* FSL_FEATURE_ADC16_HAS_DIFF_MODE */
}


void ADC0_IRQHandler(void)
{
    g_Adc16ConversionDoneFlag = true;
    /* Read conversion result to clear the conversion completed flag. */
    g_Adc16ConversionValue = ADC16_GetChannelConversionValue(DEMO_ADC16_BASEADDR, DEMO_ADC16_CHANNEL_GROUP);
}

void ADC_task(void *pvParameters)
{
	const TickType_t xDelay = 100/portTICK_PERIOD_MS;
	TickType_t xLastProcessingTime;
	xLastProcessingTime = xTaskGetTickCount();

	while(1)
	{
		Log_messenger(Debug, __func__, "Reading ADC values.");
		taskYIELD();
		//ADC conversion not done
		g_Adc16ConversionDoneFlag = false;
		//set the ADC channel
		ADC16_SetChannelConfig(DEMO_ADC16_BASEADDR, DEMO_ADC16_CHANNEL_GROUP, &g_adc16ChannelConfigStruct);

		//Wait till conversion is over
		while(!g_Adc16ConversionDoneFlag)
		{
			taskYIELD(); //request a context switch to another task
		}

		uint32_t adcval = g_Adc16ConversionValue; //adc value
		if(!uxSemaphoreGetCount(xMutex))
		{
			LED_count(); //LED update
		}

		//Send item to the queue
		if(xQueueSend(adcQueue, &adcval, 0) == errQUEUE_FULL)
		{
			Log_messenger(Debug, __func__, "ADC queue full, using DMA to transfer to DSP queue");

			Log_messenger(Normal, __func__, "ADC queue full, using DMA to transfer to DSP queue");
			//Block the LED
			xSemaphoreTake(xMutex, 10);

			//Holds the pointer to the queue
			uint32_t* queueptr;
			queueptr = adcQueue;

			//Transfer contents of the queue to the DSP buffer
			DMA_PrepareTransfer(&transferConfig, &queueptr[20], sizeof(queueptr[20]), destDMA, sizeof(destDMA[0]), 64 * sizeof(uint32_t), kDMA_MemoryToMemory);
			//Prepare and start the transfer
			DMA_SubmitTransfer(&g_DMA_Handle, &transferConfig, kDMA_EnableInterrupt);
			DMA_StartTransfer(&g_DMA_Handle);
			//		taskYIELD();

			//Wait till transfer is done
			while(g_Transfer_Done != true);
			//In case the queue is not empty, empty it
			uint32_t temp;
			while(xQueueReceive(adcQueue, &temp, 0) != errQUEUE_EMPTY);

			//If it is the 5th cycle, calculate the values and terminate the program
			if(mathCounter == 4)
			{
				++mathCounter;
				math_results();
				//Kill all tasks
				printf("Terminating Program\n");
				vTaskSuspend(dac_task_handle);
				vTaskSuspend(adc_task_handle);
				vTaskSuspend(dmaMath);

				while(1);
			}


			Log_messenger(Debug, __func__, "Resuming the DSP task");
			if(xTaskCreate(DSPMathOps, "Math Operations", configMINIMAL_STACK_SIZE + 256, NULL,tskIDLE_PRIORITY + 1, dmaMath ) == pdFALSE)
				printf("TASK CREATION FAILED\n");

			taskYIELD();
		}
		//Wait for 100mS
		vTaskDelayUntil( &xLastProcessingTime, xDelay );
	}
}
