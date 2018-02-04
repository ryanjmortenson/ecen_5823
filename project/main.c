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
#include "em_core.h"

#include "i2c.h"

//***********************************************************************************
// defined files
//***********************************************************************************

//***********************************************************************************
// global variables
//***********************************************************************************
int32_t led_state = LED0_default;
uint8_t events = 0;

//***********************************************************************************
// function prototypes
//***********************************************************************************

//***********************************************************************************
// functions
//***********************************************************************************
void
LETIMER0_IRQHandler (void)
{
  CORE_ATOMIC_IRQ_DISABLE ();

  // Clear interrupt
  LETIMER_IntClear (LETIMER0, LETIMER_INTERRUPTS);

#if LIGHT_LED_ON_TEMP_READ
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
#endif

  if (events & CREATE_EVENT (START_TEMP_SENSOR))
  {
    SET_EVENT (events, READ_TEMPERATURE);
  }

  if (!events)
  {
    SET_EVENT (events, START_TEMP_SENSOR);
  }

  CORE_ATOMIC_IRQ_ENABLE ();
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
  TEMPSENS_Temp_TypeDef temp;

  // Initialize device
  initMcu ();

  // Initialize board
  initBoard ();

  // Initialize GPIO
  gpio_init ();

  // Initialize clocks
  cmu_init ();

  // Initialize the LETIMER
  if (letimer_init (2.0, .875))
  {
    while (1)
    {
      // Sleep until an event occurs
      sleep ();

      if ((events & CREATE_EVENT (START_TEMP_SENSOR))
          && !(events & CREATE_EVENT (READ_TEMPERATURE)))
      {
        // Initialize I2C
        I2C_Tempsens_Init ();
      }

      if (events & CREATE_EVENT (READ_TEMPERATURE))
      {

        // Read temperature
        TEMPSENS_TemperatureGet (I2C0, TEMPSENS_DVK_ADDR, &temp);

        // Clear temperature event
        events &= ~(CREATE_EVENT (READ_TEMPERATURE));
        events &= ~(CREATE_EVENT (START_TEMP_SENSOR));

        // Shutdown I2C temp sensor
        I2C_Tempsens_Dest ();
      }
    }
  }
}

/** @} (end addtogroup app) */
/** @} (end addtogroup Application) */
