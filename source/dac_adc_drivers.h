/*
 * @file dac_adc_driver.c
 * @reference frdmkl25z_demo_apps_dac_adc
 *
 *
 * @authors Morgan Tilong, Xinyuan Zhoa
 * @date April 28 2020
 */


#ifndef DAC_ADC_DRIVERS_H_
#define DAC_ADC_DRIVERS_H_

#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_adc16.h"
#include "fsl_dac.h"

#include "clock_config.h"
#include "pin_mux.h"

/* FreeRTOS kernal includs */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

#include "logger.h"
#include "led.h"
#include "dma_driver.h"
#include "math_functions.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_ADC16_BASEADDR ADC0
#define DEMO_ADC16_CHANNEL_GROUP 0U
#define DEMO_ADC16_USER_CHANNEL 0U /* PTE20, ADC0_SE0 */
#define DEMO_DAC_BASEADDR DAC0

#define VREF_BRD 3.300  //max value of adc
#define SE_12BIT 4096.0 //max adc bits

/*******************************************************************************
 * Variables
 ******************************************************************************/
extern volatile bool g_Adc16ConversionDoneFlag;
extern volatile uint32_t g_Adc16ConversionValue;
extern adc16_channel_config_t g_adc16ChannelConfigStruct;

/* ADC input queue handle */
extern QueueHandle_t adcQueue;


extern TaskHandle_t dac_task_handle; //sine wave lookup values to DAC
extern TaskHandle_t adc_task_handle; //read sine values from ADC
extern uint8_t DACcounter;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* Initialize ADC16 & DAC */
void DAC_Initialize(void);
void ADC_Initialize(void);
void ADC_task(void *pvParameters);
void DAC_task(void *pvParameters);



#endif /* DAC_ADC_DRIVERS_H_ */
