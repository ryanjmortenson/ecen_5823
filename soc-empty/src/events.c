/*
 * events.c
 *
 *  Created on: Feb 18, 2018
 *      Author: mortzula
 */

#include "infrastructure.h"
#include "gatt_db.h"
#include "native_gecko.h"
#include "em_adc.h"

#include "src/main.h"
#include "src/i2c.h"
#include "src/lightsens.h"
#include "src/gpio.h"
#include "src/soil_moisture.h"


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
    LIGHTSENS_SetContinuous(I2C0);
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

    if (temp < MINIMUM_TEMP)
    {
      GPIO_PinModeSet (LED0_port, LED0_pin, gpioModePushPull, true);
    }
    else
    {
      GPIO_PinModeSet (LED0_port, LED0_pin, gpioModePushPull, false);
    }

    // Convert temp to a bitstream and sned
    utemp = FLT_TO_UINT32 ((uint32_t) (temp * 1000), -3);
    UINT8_TO_BITSTREAM (buf_start, 0);
    UINT32_TO_BITSTREAM (buf_start, utemp);
    gecko_cmd_gatt_server_send_characteristic_notification (0xFF,
                                                            gattdb_temp_measurement,
                                                            5, buffer);
    buf_start = buffer;
    UINT32_TO_BITSTREAM (buf_start, get_soil_moisture());
    gecko_cmd_gatt_server_write_attribute_value (gattdb_soil_moisture_measurement, 0, 4, buffer);

    buf_start = buffer;
    UINT32_TO_BITSTREAM (buf_start, utemp);
    gecko_cmd_gatt_server_write_attribute_value (gattdb_temperature, 0, 2, buffer);

    buf_start = buffer;
    utemp = FLT_TO_UINT32 ((uint32_t) (lux * 1000), -3);
    UINT32_TO_BITSTREAM (buf_start, utemp);
    gecko_cmd_gatt_server_write_attribute_value (gattdb_irradiance, 0, 2, buffer);
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
