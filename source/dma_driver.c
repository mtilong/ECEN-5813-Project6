
/*
 * @brief   Project 6
 *
 *Transfer data from ADC to DSP
 *
 * @date April 28th 2020
 * @author Morgan Tilong, Xinyuan Zhoa
 */

#include "dma_driver.h"
#include "logger.h"

dma_transfer_config_t transferConfig;
dma_handle_t g_DMA_Handle;
uint32_t destDMA[64] = {0};
volatile bool g_Transfer_Done;


void DMA_Callback(dma_handle_t *handle, void *param)
{
    g_Transfer_Done = true;
	//xQueueReset(adcQueue);
	Log_messenger(Debug, __func__, "DMA Transfer completed");
	Log_messenger(Normal, __func__, "DMA Transfer completed");
}

void DMA_init(void)
{
/* Configure DMAMUX */
    DMAMUX_Init(DMAMUX0);
    DMAMUX_SetSource(DMAMUX0, DMA_CHANNEL, DMA_SOURCE);
    DMAMUX_EnableChannel(DMAMUX0, DMA_CHANNEL);
    /* Configure DMA one shot transfer */
    DMA_Init(DMA0);
    DMA_CreateHandle(&g_DMA_Handle, DMA0, DMA_CHANNEL);
    DMA_SetCallback(&g_DMA_Handle, DMA_Callback, NULL);
}
