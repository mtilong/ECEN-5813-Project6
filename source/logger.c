/*
 * @brief Source file that contains the logger print functions
 *
 * Logger function print functions, strings, intergs etc.
 *
 * @authors Morgan Tilong, Xinyuan Zhoa
 * April 28th 2020
 */

#include "logger.h"

uint8_t status;
unsigned int tenth = 0;

 void Log_enable()
 {
	 status = 1;
	Log_messenger(Debug, __func__, "Enable the logger.");

 }
 void Log_disable()
 {
	 status = 0;
 }
uint8_t Log_status()
{
	 return status;
}


void Log_data(uint32_t* adress, size_t length)
{
	if (status == 1)
	{
		for(size_t i=0; i<length; i++)
		{
			//PRINTF("Memory allocated at 0x%08X Hex is %x ",(ptr),*(ptr));
			PRINTF("%p: ", adress+i);
			Log_integer_format((adress+i));

		}
		PRINTF("\n");
	}
}

void Log_messenger(enum level log_level, const char *function, char *message)
{
	if(status == 1)
	{
		//time stamp
		unsigned long hour = tenth/36000UL;
		unsigned long min = tenth/600UL;
		unsigned long sec = tenth/10UL;

		if(sec>=60)
			sec-=60;
		if(min>=60)
			min-=60;
		if(hour>=24)
			hour-=24;

//debug mode
#ifdef DEBUG
		if(log_level == Debug)
		{
			PRINTF("DEBUG: %lu:%lu:%lu:%lu ",hour, min, sec, tenth%10);
			PRINTF("%s: %s\n",function, message);
		}
#endif

//Test mode
#ifdef TEST
		if(log_level == Test)
		{
			PRINTF("TEST: ");
			PRINTF("%s: %s\n",function, message);
		}
#endif


//Normal mode
#ifdef NORMAL
		if(log_level == Normal)
		{
			PRINTF("NORMAL MODE: %lu:%lu:%lu:%lu ",hour, min, sec, tenth%10);
			PRINTF("%s: %s\n",function, message);
		}
#endif
	}

}



//print string...
void Log_string(char* s)
{	if(status ==1)
	{
		PRINTF("%s ",s);
	}
}


//print integer...
void Log_integer_format(uint32_t *adress)
{	if(status ==1)
	{
		PRINTF("d\n", *adress);
	}
}

void Init_SysTick(void)
{
	//Load value eq to 0.1 sec
	SysTick->LOAD = tenthSec;
	//Set the systick timer priority
	NVIC_SetPriority(SysTick_IRQn, 3);
	//set the value register to 0
	SysTick->VAL = 0;
	//Enable and start the timer
	SysTick->CTRL = SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}

/*
 void SysTick_Handler()
 {

 }*/
