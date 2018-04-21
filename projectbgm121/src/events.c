#include <stdint.h>

#include "infrastructure.h"
#include "gatt_db.h"
#include "native_gecko.h"
#include "em_adc.h"

#include "src/events.h"
#include "src/tempsense.h"
#include "src/lightsens.h"
#include "src/gpio.h"
#include "src/soil_moisture.h"
#include "src/persistent_data.h"
#include "src/setters.h"

#include "graphics.h"

uint16_t measurements = 0;
volatile bool sensor_started = false;

void handle_events (uint8_t * events)
{
  float temp = 0.0f;
  float lux = 0;

  int8_t temp_ret = 0;
  int8_t lux_ret = 0;

  // Handle the device init
  if ((*events & CREATE_EVENT (START_TEMP_SENSOR)))
  {
    // Clear temperature event
    *events &= ~(CREATE_EVENT (START_TEMP_SENSOR));

    // Set the sensor started flag
    sensor_started = true;

    // Initialize I2C
    I2C_Tempsens_Init ();

    // Initialize soil moisture sensor
    soil_moisture_init ();

    // Set the light sensor to continuous
    LIGHTSENS_SetSingleShot(I2C0);
  }

  if (*events & CREATE_EVENT (READ_TEMPERATURE))
  {
    // Clear temperature event
    *events &= ~(CREATE_EVENT (READ_TEMPERATURE));

    // Set the sensor started flag
    sensor_started = false;

    // Read temperature
    temp_ret = TEMPSENS_TemperatureGet (I2C0, TEMPSENS_DVK_ADDR, &temp);

    // Read ambient light
    lux_ret = LIGHTSENS_GetLux(I2C0, &lux);

    // Shutdown I2C sensors
    I2C_Tempsens_Dest ();

    // Shutdown soil moisture adc
    soil_moisture_dest();

    // Convert soil moisture into bit-stream and send
    soil_moisture_setter(get_soil_moisture());

    // Convert temperature to a bit-stream and send
    if (temp_ret == 0)
    {
      temp_setter(temp);
    }

    if (lux_ret == 0)
    {
      lux_setter(lux);
    }

    // Increment measurements and set for gattdb
    measurements++;
    save(MEASUREMENT_COUNT_KEY, measurements);
    measurement_setter(measurements);
  }

  if (*events & CREATE_EVENT (READ_SOIL_MOISTURE))
  {
    // Reset event and handle
    *events &= ~(CREATE_EVENT (READ_SOIL_MOISTURE));
    handle_soil_moisture_event();
  }
}

void set_events (uint8_t * events)
{
  if (!sensor_started)
  {
    SET_EVENT (*events, START_TEMP_SENSOR);
  }
  else
  {
    SET_EVENT (*events, READ_TEMPERATURE);
  }
}
