#ifndef SRC_BLE_SETTINGS_H_
#define SRC_BLE_SETTINGS_H_

#include "gecko_configuration.h"

#define ADV_INT (1000) // 4 seconds
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

#endif /* SRC_BLE_SETTINGS_H_ */
