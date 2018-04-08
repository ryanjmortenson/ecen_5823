#include <stdint.h>

#include "infrastructure.h"
#include "gatt_db.h"
#include "native_gecko.h"
#include "em_adc.h"

#include "src/main.h"
#include "src/i2c.h"
#include "src/lightsens.h"
#include "src/gpio.h"
#include "src/soil_moisture.h"
#include "src/persistent_data.h"

#include "graphics.h"

uint16_t measurements = 0;

void handle_events (uint8_t * events)
{
  float temp = 0.0f;
  float lux = 0;
  uint32_t utemp = 0;
  uint8_t buffer[5] = { 0 };
  uint8_t *buf_start = buffer;

  // Handle the device init
  if ((*events & CREATE_EVENT (START_TEMP_SENSOR))
      && !(*events & CREATE_EVENT (READ_TEMPERATURE)))
  {
    // Initialize I2C
    I2C_Tempsens_Init ();

    // Initialize soil moisture sensor
    soil_moisture_init ();

    // Set the light sensor to continuous
    LIGHTSENS_SetSingleShot(I2C0);
  }

  if (*events & CREATE_EVENT (READ_TEMPERATURE))
  {
    // Read temperature
    TEMPSENS_TemperatureGet (I2C0, TEMPSENS_DVK_ADDR, &temp);

    // Read ambient light
    LIGHTSENS_GetLux(I2C0, &lux);

    // Shutdown I2C sensors
    I2C_Tempsens_Dest ();

    // Clear temperature event
    *events &= ~(CREATE_EVENT (READ_TEMPERATURE));
    *events &= ~(CREATE_EVENT (START_TEMP_SENSOR));

    // Convert soil moisture into bit-stream and send
    buf_start = buffer;
    UINT32_TO_BITSTREAM (buf_start, get_soil_moisture());
    gecko_cmd_gatt_server_write_attribute_value (gattdb_soil_moisture_measurement, 0, 4, buffer);

    // Convert temperature to a bit-stream and send
    buf_start = buffer;
    utemp = FLT_TO_UINT32 ((uint32_t) (temp * 1000), -3);
    UINT32_TO_BITSTREAM (buf_start, utemp);
    gecko_cmd_gatt_server_write_attribute_value (gattdb_temperature, 0, 2, buffer);

    // Convert irradiance to bit stream and send
    buf_start = buffer;
    utemp = FLT_TO_UINT32 (lux, 0);
    UINT32_TO_BITSTREAM (buf_start, utemp);
    gecko_cmd_gatt_server_write_attribute_value (gattdb_irradiance, 0, 2, buffer);


    // Increment measurements and set for gattdb
    buf_start = buffer;
    measurements++;
    save(MEASUREMENT_COUNT_KEY, measurements);
    buf_start = buffer;
    UINT16_TO_BITSTREAM (buf_start, measurements);
    gecko_cmd_gatt_server_write_attribute_value (gattdb_measurement_count, 0, 4, buffer);
  }
}

void set_events (uint8_t * events)
{
  if (*events & CREATE_EVENT (START_TEMP_SENSOR))
  {
    SET_EVENT (*events, READ_TEMPERATURE);
  }

  if (!*events)
  {
    SET_EVENT (*events, START_TEMP_SENSOR);
  }
}
