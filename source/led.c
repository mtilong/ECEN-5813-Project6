/*
 *
 * LED Initiate BLUE, RED, GREEN and OFF functions
 *
 * @authors Morgan Tilong, Xinyuan Zhoa
 * @ April 28th 2020
 */

#include "led.h"

SemaphoreHandle_t xMutex;
TaskHandle_t dma_led_task_handle;


uint16_t led_count; //number of time LED blinks

void KL25Z_RGB_Flasher(void)
{
	// Enable clock to Port B and Port D
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTD_MASK;

	// Make 3 pins GPIO
	//First pin to GPIO
	PORTB->PCR[RED_LED_SHIFT] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[RED_LED_SHIFT] |= PORT_PCR_MUX(1);

	//Second pin to GPIO
	PORTB->PCR[GREEN_LED_SHIFT] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[GREEN_LED_SHIFT] |= PORT_PCR_MUX(1);

	//Third pin to GPIO
	PORTD->PCR[BLUE_LED_SHIFT] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[BLUE_LED_SHIFT] |= PORT_PCR_MUX(1);

	// Set ports to outputs
	PTB->PDDR |= (MASK(RED_LED_SHIFT) | MASK(GREEN_LED_SHIFT));
	PTD->PDDR |= MASK(BLUE_LED_SHIFT);

	//turn every light on
	PTB->PCOR |= MASK(RED_LED_SHIFT) | MASK(GREEN_LED_SHIFT);
	PTD->PCOR |= MASK(BLUE_LED_SHIFT);

	//turn every led off
	PTB->PSOR |= MASK(RED_LED_SHIFT) | MASK(GREEN_LED_SHIFT);
	PTD->PSOR |= MASK(BLUE_LED_SHIFT);

	Log_messenger(DEBUG, __func__, "Initializing LEDs");

}


void delay2(void)
{
	//(ms*cpufreq_Mhz*1000)/10.. CPUfreq = 48MHz

	int loopcount = 14000000;
	while(1)  //always true
	{
		loopcount--;  //decrement duration
		if(loopcount <= 0) //time reached
			break;
	}
}


void LED_off()
{
//turn every led off
	PTB->PSOR |= MASK(RED_LED_SHIFT) | MASK(GREEN_LED_SHIFT);
	PTD->PSOR |= MASK(BLUE_LED_SHIFT);
	Log_messenger(DEBUG, __func__, "Turn LEDs off.");

}

//Turn Green On
void LED_cyclegreen_On()
{
	LED_off();
	PTB->PCOR = MASK(GREEN_LED_SHIFT);//on
	Log_messenger(DEBUG, __func__, "Turn Green LED on.");

}

//Turn Red On
void LED_cyclered_On()
{
	LED_off();
	PTB->PCOR = MASK(RED_LED_SHIFT);//on
	Log_messenger(DEBUG, __func__, "Turn Red LED on.");

}

//Turn Blue On
void LED_cycleblue_On()
{
	LED_off();
	PTD->PCOR = MASK(BLUE_LED_SHIFT);//on;
	Log_messenger(DEBUG, __func__, "Turn Blue LED on.");
}

void LED_count()
{
	led_count++;
	LED_cycleblue_On();

	if(led_count == 5)
	{
		led_count = 0; //reset led count;
		xSemaphoreGive(xMutex);
		LED_off();
		taskYIELD();
	}
}
