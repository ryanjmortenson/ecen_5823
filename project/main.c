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
int led_state = LED0_default;

//***********************************************************************************
// function prototypes
//***********************************************************************************

//***********************************************************************************
// functions
//***********************************************************************************
void
LETIMER0_IRQHandler (void)
{
  // Clear interrupt
  LETIMER_IntClear (LETIMER0, LETIMER_INTERRUPTS);

  // Set or clear pin based on led_state
  if (led_state)
  {
    GPIO_PinOutClear (LED0_port, LED0_pin);
  }
  else
  {
    GPIO_PinOutSet (LED0_port, LED0_pin);
  }

  // Switch led state
  led_state = led_state ? false : true;
}

//***********************************************************************************
// main
//***********************************************************************************
/**
 * @brief  Main function
 */
int
main (void)
{
  // Initialize device
  initMcu ();

  // Initialize board
  initBoard ();

  // Initialize GPIO
  gpio_init ();

  // Initialize clocks
  cmu_init ();

  // Initialize the LETIMER
  if (letimer_init (PERIOD, DUTY_CYCLE))
  {
    // Set the lowest EM to EM_CANT_ENTER
    block_sleep_mode (EM_CANT_ENTER);
    while (1)
    {
      sleep ();
    }
  }
}

/** @} (end addtogroup app) */
/** @} (end addtogroup Application) */
