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
#include "init_app.h"
#include "ble-configuration.h"
#include "board_features.h"

/* Bluetooth stack headers */
#include "bg_types.h"
#include "native_gecko.h"
#include "gatt_db.h"

/* Libraries containing default Gecko configuration values */
#include "em_emu.h"
#include "em_cmu.h"
#include "em_letimer.h"
#include "em_core.h"
#include "infrastructure.h"

/* Device initialization header */
#include "hal-config.h"

#if defined(HAL_CONFIG)
#include "bsphalconfig.h"
#else
#include "bspconfig.h"
#endif

#include "src/gpio.h"
#include "src/letimer.h"
#include "src/cmu.h"
#include "src/i2c.h"
#include "src/power_level.h"
#include "src/events.h"

int led_state = LED0_default;
uint8_t events = 0;

/***********************************************************************************************//**
 * @addtogroup Application
 * @{
 **************************************************************************************************/

/***********************************************************************************************//**
 * @addtogroup app
 * @{
 **************************************************************************************************/

#define SAMPLE_PERIOD (4.0f)
#define SENSOR_INIT_TIME (.080f)
#define CALCULATE_INIT_DUTY_CYCLE(init_time) ((SAMPLE_PERIOD - init_time) / SAMPLE_PERIOD)
#define ADV_INT (539)
#define CONN_INTERVAL (60) // 75 / 1.25
#define SLAVE_LATENCY (5)  // 5 * 75 + 75 = 450
#define TIMEOUT (1000)

#ifndef MAX_CONNECTIONS
#define MAX_CONNECTIONS 4
#endif
uint8_t bluetooth_stack_heap[DEFAULT_BLUETOOTH_HEAP (MAX_CONNECTIONS)];

// Gecko configuration parameters (see gecko_configuration.h)
static const gecko_configuration_t config =
{
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
  .pa.input = GECKO_RADIO_PA_INPUT_VBAT,// Configure PA input to VBAT
#endif // (HAL_PA_ENABLE) && defined(FEATURE_PA_HIGH_POWER)
};

// Flag for indicating DFU Reset must be performed
uint8_t boot_to_dfu = 0;

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

  // Set the events
  set_events (&events);

  CORE_ATOMIC_IRQ_ENABLE ();
}

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

  // Initialize application
  initApp ();

  // Initialize GPIO
  gpio_init ();

  // Initialize clocks
  cmu_init ();

  // Initialize stack
  gecko_init (&config);

  if (letimer_init (PERIOD, DUTY_CYCLE))
  {
    // Hold current temperature reading
    uint8_t conn;

    while (1)
    {
      /* Event pointer for handling events */
      struct gecko_cmd_packet* evt;

      /* Check for stack event. */
      evt = gecko_wait_event ();

      /* Handle events */
      switch (BGLIB_MSG_ID (evt->header))
      {
        /* This boot event is generated when the system boots up after reset.
         * Do not call any stack commands before receiving the boot event.
         * Here the system is set to start advertising immediately after boot procedure. */
        case gecko_evt_system_boot_id:
          /* Set advertising parameters. 100ms advertisement interval. All channels used.
           * The first two parameters are minimum and maximum advertising interval, both in
           * units of (milliseconds * 1.6). The third parameter '7' sets advertising on all channels. */
          gecko_cmd_le_gap_set_adv_parameters (ADV_INT, ADV_INT, 7);

          /* Start general advertising and enable connections. */
          gecko_cmd_le_gap_set_mode (le_gap_general_discoverable,
                                     le_gap_undirected_connectable);

          gecko_cmd_system_set_tx_power (0);

          break;

        case gecko_evt_le_connection_opened_id:
          gecko_cmd_le_connection_set_parameters (
              evt->data.evt_le_connection_opened.connection, CONN_INTERVAL,
              CONN_INTERVAL,
              SLAVE_LATENCY, TIMEOUT);

          conn = evt->data.evt_le_connection_opened.connection;
          break;

        case gecko_evt_le_connection_closed_id:
          // Reset the connection
          conn = 0;
          gecko_cmd_system_set_tx_power (0);

          /* Check if need to boot to dfu mode */
          if (boot_to_dfu)
          {
            /* Enter to DFU OTA mode */
            gecko_cmd_system_reset (2);
          }
          else
          {
            /* Restart advertising after client has disconnected */
            gecko_cmd_le_gap_set_mode (le_gap_general_discoverable,
                                       le_gap_undirected_connectable);
          }
          break;

          /* Events related to OTA upgrading
           ----------------------------------------------------------------------------- */

          /* Check if the user-type OTA Control Characteristic was written.
           * If ota_control was written, boot the device into Device Firmware Upgrade (DFU) mode. */
        case gecko_evt_gatt_server_user_write_request_id:

          if (evt->data.evt_gatt_server_user_write_request.characteristic
              == gattdb_ota_control)
          {
            /* Set flag to enter to OTA mode */
            boot_to_dfu = 1;
            /* Send response to Write Request */
            gecko_cmd_gatt_server_send_user_write_response (
                evt->data.evt_gatt_server_user_write_request.connection,
                gattdb_ota_control, bg_err_success);

            /* Close connection to enter to DFU OTA mode */
            gecko_cmd_endpoint_close (
                evt->data.evt_gatt_server_user_write_request.connection);
          }
          break;

        case gecko_evt_system_external_signal_id:
          // If connection is active set rssi
          if (conn)
          {
            gecko_cmd_le_connection_get_rssi (conn);
          }

          // Handle set of events
          handle_events (&events);

          break;

        case gecko_evt_le_connection_rssi_id:
          // Set the power level based on RSSI
          gecko_cmd_system_set_tx_power (
              get_power_level (evt->data.evt_le_connection_rssi.rssi));
          break;

        default:
          break;
      }
    }
  }
}

/** @} (end addtogroup app) */
/** @} (end addtogroup Application) */
