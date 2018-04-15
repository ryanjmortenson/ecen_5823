/***********************************************************************************************
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

#include <stdio.h>
#include <inttypes.h>

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

#include "graphics.h"
#include "sleep.h"

#include "src/gpio.h"
#include "src/letimer.h"
#include "src/cmu.h"
#include "src/tempsense.h"
#include "src/power_level.h"
#include "src/events.h"
#include "src/soil_moisture.h"
#include "src/persistent_data.h"

int led_state = LED0_default;
uint8_t events = 0;

/***********************************************************************************************
 * @addtogroup Application
 * @{
 **************************************************************************************************/

/***********************************************************************************************
 * @addtogroup app
 * @{
 **************************************************************************************************/

#define SAMPLE_PERIOD (120.0f) // 2 minutes
#define SENSOR_INIT_TIME (.120f) // 120 milliseconds for sensors to fully initialize
#define CALCULATE_INIT_DUTY_CYCLE(init_time) ((SAMPLE_PERIOD - init_time) / SAMPLE_PERIOD)
#define ADV_INT (6500) // 4 seconds
#define CONN_INTERVAL (6) // Minimum
#define SLAVE_LATENCY (0) // Minimum
#define TIMEOUT (1000)


#ifndef MAX_CONNECTIONS
#define MAX_CONNECTIONS 4
#endif
uint8_t bluetooth_stack_heap[DEFAULT_BLUETOOTH_HEAP (MAX_CONNECTIONS)];

// Gecko configuration parameters (see gecko_configuration.h)
static const gecko_configuration_t config = {
  .config_flags = 0,
  .sleep.flags = SLEEP_FLAGS_DEEP_SLEEP_ENABLE,
  .bluetooth.max_connections = MAX_CONNECTIONS,
  .bluetooth.heap = bluetooth_stack_heap,
  .bluetooth.heap_size = sizeof (bluetooth_stack_heap),
  .bluetooth.sleep_clock_accuracy = 100,  // ppm
  .gattdb = &bg_gattdb_data,
  .ota.flags = 0,
  .ota.device_name_len = 3,
  .ota.device_name_ptr = "OTA",
#if (HAL_PA_ENABLE) && defined(FEATURE_PA_HIGH_POWER)
  .pa.config_enable = 1,  // Enable high power PA
  .pa.input = GECKO_RADIO_PA_INPUT_VBAT,  // Configure PA input to VBAT
#endif // (HAL_PA_ENABLE) && defined(FEATURE_PA_HIGH_POWER)
};

// Flag for indicating DFU Reset must be performed
uint8_t boot_to_dfu = 0;
uint32_t connections = 0;
extern uint32_t measurements;

void LETIMER0_IRQHandler (void)
{
  CORE_ATOMIC_IRQ_DISABLE ();

  // Clear interrupt
  LETIMER_IntClear (LETIMER0, LETIMER_INTERRUPTS);

  // Set the events
  set_events (&events);

  // Signal events with gecko system
  gecko_external_signal (events);

  CORE_ATOMIC_IRQ_ENABLE ();
}

/**
 * @brief  Main function
 */
int main (void)
{
#ifdef SECURITY_ON
  // Buffer for holding PK print statement
  char passbuffer[32] = {0};
#endif
  uint8_t buffer[2] = {0};
  uint8_t* buf_start = buffer;

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

  if (letimer_init (SAMPLE_PERIOD, CALCULATE_INIT_DUTY_CYCLE (SENSOR_INIT_TIME)))
  {
    while (1)
    {
      /* Event pointer for handling events */
      struct gecko_cmd_packet *evt;

      /* Check for stack event. */
      evt = gecko_wait_event ();

      /* Handle events */
      switch (BGLIB_MSG_ID (evt->header))
      {
        /* This boot event is generated when the system boots up after reset.
         * Do not call any stack commands before receiving the boot event. Here 
         * the system is set to start advertising immediately after boot
         * procedure. */
      case gecko_evt_system_boot_id:
#ifdef SECURITY_ON
	// Set up security features
	gecko_cmd_sm_delete_bondings();
	gecko_cmd_sm_configure(0x0F, sm_io_capability_displayonly);
	gecko_cmd_sm_set_bondable_mode(1);
#endif

#ifdef ERASE_ALL_PERSISTENT_DATA
        // Erase all persistent data (reset)
        gecko_cmd_flash_ps_erase_all();
#endif
        /* Set advertising parameters. 100ms advertisement interval. All
         * channels used. The first two parameters are minimum and maximum
         * advertising interval, both in units of (milliseconds * 1.6). The
         * third parameter '7' sets advertising on all channels. */
        gecko_cmd_le_gap_set_adv_parameters (ADV_INT, ADV_INT, 7);

        /* Start general advertising and enable connections. */
        gecko_cmd_le_gap_set_mode (le_gap_general_discoverable,
                                   le_gap_undirected_connectable);

        // Set tx power to 0
        gecko_cmd_system_set_tx_power (0);


        // Load or set the connections persistent data
        load_or_set_initial(CONNECTION_COUNT_KEY, 0, &connections);

        buf_start = buffer;
        UINT32_TO_BITSTREAM (buf_start, connections);
        gecko_cmd_gatt_server_write_attribute_value (gattdb_connection_count, 0, 4, buffer);

        // Load or set the measurement in the boot, measurement counts will be handled in events.c
	load_or_set_initial(MEASUREMENT_COUNT_KEY, 0, &measurements);

	buf_start = buffer;
	UINT32_TO_BITSTREAM (buf_start, measurements);
	gecko_cmd_gatt_server_write_attribute_value (gattdb_measurement_count, 0, 4, buffer);
        break;

      case gecko_evt_le_connection_opened_id:
        gecko_cmd_le_connection_set_parameters (evt->data.
                                                evt_le_connection_opened.
                                                connection, CONN_INTERVAL,
                                                CONN_INTERVAL, SLAVE_LATENCY,
                                                TIMEOUT);

        // Set connection power
        gecko_cmd_le_connection_get_rssi (evt->data.evt_le_connection_opened.connection);

        // Increment connections and set in gattdb
        connections++;
        save(CONNECTION_COUNT_KEY, connections);
        buf_start = buffer;
	UINT32_TO_BITSTREAM (buf_start, connections);
	gecko_cmd_gatt_server_write_attribute_value (gattdb_connection_count, 0, 4, buffer);

#ifdef SECURITY_ON
        /* The HTM service typically indicates and indications cannot be given an encrypted property so
        force encryption immediately after connecting */
        gecko_cmd_sm_increase_security(evt->data.evt_le_connection_opened.connection);
#endif
        break;

#ifdef SECURITY_ON
      case gecko_evt_sm_passkey_display_id:
	snprintf(passbuffer, 32, "PK: %06"PRIu32, evt->data.evt_sm_passkey_display.passkey);
        GRAPHICS_Init();
	GRAPHICS_Clear();
	GRAPHICS_AppendString(passbuffer);
	GRAPHICS_Update();
	memset(passbuffer, 0, 32);
	break;

      case gecko_evt_sm_bonded_id:
	GRAPHICS_Clear();
	GRAPHICS_Sleep();
	break;

      case gecko_evt_sm_bonding_failed_id:
	GRAPHICS_AppendString("\nBond Failed");
	GRAPHICS_Update();
	break;
#endif

      case gecko_evt_le_connection_closed_id:
        // Reset the connection
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
         * ----------------------------------------------------------------------------- 
         */

        /* Check if the user-type OTA Control Characteristic was written. If
         * ota_control was written, boot the device into Device Firmware
         * Upgrade (DFU) mode. */
      case gecko_evt_gatt_server_user_write_request_id:

        if (evt->data.evt_gatt_server_user_write_request.characteristic
            == gattdb_ota_control)
        {
          /* Set flag to enter to OTA mode */
          boot_to_dfu = 1;
          /* Send response to Write Request */
          gecko_cmd_gatt_server_send_user_write_response (evt->data.
                                                          evt_gatt_server_user_write_request.
                                                          connection,
                                                          gattdb_ota_control,
                                                          bg_err_success);

          /* Close connection to enter to DFU OTA mode */
          gecko_cmd_endpoint_close (evt->data.
                                    evt_gatt_server_user_write_request.
                                    connection);
        }
        break;

      case gecko_evt_system_external_signal_id:
        // Handle set of events
        handle_events (&events);
        break;

      case gecko_evt_le_connection_rssi_id:
        // Set the power level based on RSSI
        gecko_cmd_system_set_tx_power (get_power_level
                                       (evt->data.evt_le_connection_rssi.rssi));
        break;

      default:
        break;
      }
    }
  }
}

/** @} (end addtogroup app) */
/** @} (end addtogroup Application) */
