/*
 * @brief Source file that contains the logger print functions
 *
 * Generating Look Up Table, DSP math operations, math results.
 *
 * @authors Morgan Tilong, Xinyuan Zhoa
 * April 28th 2020
 */
#include <math_functions.h>

uint16_t sinDAC[50] ={0};
uint16_t mathCounter = 0;
TaskHandle_t dmaMath;

TaskHandle_t dac_task_handle; //sine wave lookup values to DAC
TaskHandle_t adc_task_handle; //read sine values from ADC


//Generate the sine Look Up Table.
//note: y = a sin (bx + c)
//note: y = 3 sin (5) (with a .1 second step)
/*
 * a = amplitude (3V)
 * b = period (5 seconds)
 * x = phase(0)
 */
void sine_wave_LUT(void)
{
	//holds sin values (-1 to 1)
	double sinLUT[50] = {0};
	for(int i = 0; i < 50; i++){
		//fills a full sin wave of 50 points from 0 to 1
		sinLUT[i] = sin((double)PI * (i / (double)24.5));  //y = sin(pi*(i/24.5))

		//Holds the scaled sin values for the DAC (in range of 1V - 3V)
		sinDAC[i] = (uint16_t)(((double)(sinLUT[i] + (double)1)/(double)2 * ((double)2479)) + (double)1239);
	}
}

//DSP buff math operations
void DSPMathOps(void *pvParameters)
{
	while(1)
	{
		if(g_Transfer_Done == true)
		{
			mathCounter++;
			if(mathCounter == 5)
			{
				printf("Terminate Task\n");
				vTaskSuspend(dac_task_handle);
				vTaskSuspend(adc_task_handle);
				vTaskSuspend(dmaMath);
				while(1);
			}
			else
			{
				math_results();
				//Suspend the program
				vTaskSuspend(dmaMath);

			}
		}
	}
}

void math_results(void)
{
	Log_messenger(Debug, __func__, "Printing DSP results:");
	Log_messenger(Normal, __func__, "Printing DSP results:");
	int i;
	//Maximum value
	uint32_t max = destDMA[0];
	uint32_t min = destDMA[0];
	//Average value
	float average = destDMA[0];
	//Standard deviation
	float sd = 0;
	//	uint32_t max1 = sinDAC[0], min1 = sinDAC[0];
	for ( i = 1 ; i < 64 ; i++)
	{
		//Get maximum
		if(destDMA[i] > max)
			max = destDMA[i];
		//Get minimum
		if(destDMA[i] < min)
			min = destDMA[i];
		//Get average
		average += destDMA[i];
	}

	average /= 64;
	//calculate standard deviation
	for (i = 0; i < 64; i++)
		sd += pow(destDMA[i] - average, 2);
	sd /= 64;
	sd = sqrt(sd);

	//Print the values
	PRINTF("Count = %d\n", mathCounter);
	PRINTF("Maximum = %d\n",max);
	PRINTF("Minimum = %d\n",min);
	PRINTF("Average = %f\n",average);
	PRINTF("Standard Deviation = %f\n",sd);
}
