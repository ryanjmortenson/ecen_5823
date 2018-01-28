/***********************************************************************************************//**
 * \file   main.c
 * \brief  Silicon Labs Empty Example Project
 *
 * This example demonstrates the bare minimum needed for a Blue Gecko C application
 * that allows Over-the-Air Device Firmware Upgrading (OTA DFU). The application
 * starts advertising after boot and restarts advertising after a connection is closed.
 ***************************************************************************************************
 * <b> (C) Copyright 2016 Silicon Labs, http://www.silabs.com</b>
 ***************************************************************************************************
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 **************************************************************************************************/

/* Board headers */
#include "init_mcu.h"
#include "init_board.h"
#include "ble-configuration.h"
#include "board_features.h"

/* Bluetooth stack headers */
#include "bg_types.h"
#include "native_gecko.h"
#include "gatt_db.h"

/* Libraries containing default Gecko configuration values */
#include "em_emu.h"
#include "em_cmu.h"

/* Device initialization header */
#include "hal-config.h"

#if defined(HAL_CONFIG)
#include "bsphalconfig.h"
#else
#include "bspconfig.h"
#endif


/***********************************************************************************************//**
 * @addtogroup Application
 * @{
 **************************************************************************************************/

/***********************************************************************************************//**
 * @addtogroup app
 * @{
 **************************************************************************************************/

#ifndef MAX_CONNECTIONS
#define MAX_CONNECTIONS 4
#endif
uint8_t bluetooth_stack_heap[DEFAULT_BLUETOOTH_HEAP(MAX_CONNECTIONS)];

// Gecko configuration parameters (see gecko_configuration.h)
static const gecko_configuration_t config = {
  .config_flags = 0,
  .sleep.flags = SLEEP_FLAGS_DEEP_SLEEP_ENABLE,
  .bluetooth.max_connections = MAX_CONNECTIONS,
  .bluetooth.heap = bluetooth_stack_heap,
  .bluetooth.heap_size = sizeof(bluetooth_stack_heap),
  .bluetooth.sleep_clock_accuracy = 100, // ppm
  .gattdb = &bg_gattdb_data,
  .ota.flags = 0,
  .ota.device_name_len = 3,
  .ota.device_name_ptr = "OTA",
#if (HAL_PA_ENABLE) && defined(FEATURE_PA_HIGH_POWER)
  .pa.config_enable = 1, // Enable high power PA
  .pa.input = GECKO_RADIO_PA_INPUT_VBAT, // Configure PA input to VBAT
#endif // (HAL_PA_ENABLE) && defined(FEATURE_PA_HIGH_POWER)
};

// Flag for indicating DFU Reset must be performed
uint8_t boot_to_dfu = 0;
//***********************************************************************************
// Include files
//***********************************************************************************

#include "main.h"
#include "gpio.h"
#include "cmu.h"
#include "letimer.h"
#include "sleep_mode.h"


#include "em_gpio.h"
#include "em_letimer.h"

//***********************************************************************************
// defined files
//***********************************************************************************


//***********************************************************************************
// global variables
//***********************************************************************************


//***********************************************************************************
// function prototypes
//***********************************************************************************


//***********************************************************************************
// functions
//***********************************************************************************


//***********************************************************************************
// main
//***********************************************************************************
void LETIMER_setup(void)

{
  /* Enable necessary clocks */

  CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);

  CMU_ClockEnable(cmuClock_CORELE, true);

  CMU_ClockEnable(cmuClock_LETIMER0, true);

  CMU_ClockEnable(cmuClock_GPIO, true);



  /* Repetition values must be nonzero so that the outputs

     return switch between idle and active state */

  LETIMER_RepeatSet(LETIMER0, 0, 0x01);

  LETIMER_RepeatSet(LETIMER0, 1, 0x01);





                /* Set up PF6 pin */

    // This configuration is valid only if PF6 and PF6 are

    // used for output signal. Change this "LOC30" value to

    // preferred value that can be found from Device datasheet

    // section Alternate Functionality Pinout.



                LETIMER0->ROUTELOC0 = (LETIMER0->ROUTELOC0

                                                & ~_LETIMER_ROUTELOC0_OUT0LOC_MASK)

                                                | LETIMER_ROUTELOC0_OUT0LOC_LOC30;

                LETIMER0->ROUTEPEN |= LETIMER_ROUTEPEN_OUT0PEN;



                /* Set up PF7 pin */

                LETIMER0->ROUTELOC0 = (LETIMER0->ROUTELOC0

                                                & ~_LETIMER_ROUTELOC0_OUT1LOC_MASK)

                                                | LETIMER_ROUTELOC0_OUT1LOC_LOC30;

                LETIMER0->ROUTEPEN |= LETIMER_ROUTEPEN_OUT1PEN;



                // [LETIMER0 I/O setup]$



  /* Set configurations for LETIMER 0 */

  const LETIMER_Init_TypeDef letimerInit =

  {

  .enable         = true,                   /* Start counting when init completed. */

  .debugRun       = false,                  /* Counter shall not keep running during debug halt. */

  .comp0Top       = true,                   /* Load COMP0 register into CNT when counter underflows. COMP0 is used as TOP */

  .bufTop         = false,                  /* Don't load COMP1 into COMP0 when REP0 reaches 0. */

  .out0Pol        = 0,                      /* Idle value for output 0. */

  .out1Pol        = 0,                      /* Idle value for output 1. */

  .ufoa0          = letimerUFOAPwm,         /* PWM output on output 0 */

  .ufoa1          = letimerUFOAPwm,       /* PWM output on output 1*/

  .repMode        = letimerRepeatFree       /* Count until stopped */

  };



  /* Initialize LETIMER */

  LETIMER_Init(LETIMER0, &letimerInit);



  // LETimer is 16 bit long and oscillator frequency is 32768Hz.

  // PWM duty-cycle and frequency can be determined using these compare value setting functions.

  // Shortly the first one determines frequency and second one determines duty-cycle. With this

  // Topvalue the minimum duty-cycle change is 1/500=0.2%.

  LETIMER_CompareSet(LETIMER0, 0, 500);

  LETIMER_CompareSet(LETIMER0, 1, 400);

}
/**
 * @brief  Main function
 */
int main(void)
{
  // Initialize device
  initMcu();

  // Initialize board
  initBoard();

  /* Initialize GPIO */
  gpio_init();

  // Initialize clocks
  //cmu_init();

  // Initialize stack
  gecko_init(&config);

  // Initialize the LETIMER
  LETIMER_setup();

  LETIMER_IntEnable(LETIMER0, LETIMER_IF_UF);

  /* Enable LETIMER0 interrupt vector in NVIC*/
  NVIC_EnableIRQ(LETIMER0_IRQn);

  while (1)
  {
	  EMU_EnterEM2(false);
  }
}

/** @} (end addtogroup app) */
/** @} (end addtogroup Application) */
