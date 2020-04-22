#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_dma.h"
#include "fsl_dmamux.h"

#include "pin_mux.h"
#include "clock_config.h"

#ifndef DMA_DRIVER_H_
#define DMA_DRIVER_H_
/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define BUFF_LENGTH 64
#define DMA_CHANNEL 0
#define DMA_SOURCE 63

/*******************************************************************************
 * Variables
 ******************************************************************************/
extern dma_handle_t g_DMA_Handle;
extern volatile bool g_Transfer_Done;
extern dma_transfer_config_t transferConfig;
//buffer transferred to by DMA
extern uint32_t destDMA[64];


/*******************************************************************************
 * Functions
 ******************************************************************************/
/*
 * @brief DMA callback function
 * Flag is set when the DMA function is done.
 *
 * The function parameter is the void pointer.
 * @return pointer void.
 */
void DMA_Callback(dma_handle_t *handle, void *param);
/*
 * @brief DMA initialization function
 * Configure the DMA to transfer 64 bytes from the ADC to DSP buffer.

 *
 * The function parameter is void.
 * @return void.
 */
void DMA_init(void);

#endif /*DMA_DRIVER_H_*/
