#include "gatt_db.h"
#include "native_gecko.h"
#include "infrastructure.h"


void connection_setter(uint32_t connections)
{
  uint8_t buffer[4] = {0};
  uint8_t * buf_start = buffer;

  UINT32_TO_BITSTREAM (buf_start, connections);
  gecko_cmd_gatt_server_write_attribute_value (gattdb_connection_count, 0, 4, buffer);
}

void measurement_setter(uint32_t measurements)
{
  uint8_t buffer[4] = {0};
  uint8_t * buf_start = buffer;

  UINT32_TO_BITSTREAM (buf_start, measurements);
  gecko_cmd_gatt_server_write_attribute_value (gattdb_measurement_count, 0, 4, buffer);
}

void soil_moisture_setter(uint32 soil_moisture)
{
  uint8_t buffer[4] = {0};
  uint8_t * buf_start = buffer;

  UINT32_TO_BITSTREAM (buf_start, soil_moisture);
  gecko_cmd_gatt_server_write_attribute_value (gattdb_soil_moisture_measurement, 0, 4, buffer);
}

void temp_setter(float temp)
{
  uint8_t buffer[2] = {0};
  uint8_t * buf_start = buffer;
  uint32_t utemp = 0;

  utemp = FLT_TO_UINT32 ((uint32_t) (temp * 1000), -3);
  UINT32_TO_BITSTREAM (buf_start, utemp);
  gecko_cmd_gatt_server_write_attribute_value (gattdb_temperature, 0, 2, buffer);
}

void lux_setter(float lux)
{
  uint8_t buffer[2] = {0};
  uint8_t * buf_start = buffer;
  uint32_t utemp = 0;

  utemp = FLT_TO_UINT32 (lux, 0);
  UINT32_TO_BITSTREAM (buf_start, utemp);
  gecko_cmd_gatt_server_write_attribute_value (gattdb_irradiance, 0, 2, buffer);
}


