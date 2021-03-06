#include <math.h>

#include "em_i2c.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_gpio.h"
#include "bsp.h"
#include "i2c.h"
#include "lightsens.h"
#include "sleep.h"


#define EXPONENT_MASK (0xf000)
#define EXPONENT_SHIFT (12)
#define FRACTIONAL_MASK (0x0fff)
#define CONV_MANTISSA (2)
#define CONV_SCALE_FACTOR (0.01f)

/*******************************************************************************
 **************************   GLOBAL VARIABLES   *******************************
 ******************************************************************************/
extern I2C_TransferReturn_TypeDef I2C_Status;

/*******************************************************************************
 **************************   GLOBAL FUNCTIONS   *******************************
 ******************************************************************************/
int LIGHTSENS_GetLux (I2C_TypeDef * i2c, float * lux)
{
  uint16_t val = 0;
  int ret = 0;

  ret = LIGHTSENS_RegisterGet (I2C0, LIGHTSENS_ADDR, lightsensRegResult, &val);
  *lux = CONV_SCALE_FACTOR * pow(CONV_MANTISSA, ((val & EXPONENT_MASK) >> EXPONENT_SHIFT)) * (val & FRACTIONAL_MASK);
  return ret;
}

int LIGHTSENS_SetSingleShot (I2C_TypeDef * i2c)
{
  uint16_t config;
  int ret = 0;

  // Read the config to update
  for (uint8_t i = 0; i < 255; i++)
  {
    ret = LIGHTSENS_RegisterGet(i2c, LIGHTSENS_ADDR, lightsensRegConfig, &config);
    if (ret == 0)
    {
      break;
    }
  }

  // Turn on continuous config
  config |= SINGLE_SHOT;

  // Turn off 100 ms conversion
  config &= ~(1 << 11);

  // Set the config
  return LIGHTSENS_RegisterSet(i2c, LIGHTSENS_ADDR, lightsensRegConfig, config);
}

int
LIGHTSENS_RegisterGet (I2C_TypeDef * i2c, uint8_t addr,
                      LIGHTSENS_Register_TypeDef reg, uint16_t * val)
{
  I2C_TransferSeq_TypeDef seq;
  uint8_t regid[1];
  uint8_t data[2];
  const uint8_t MAX_COUNT = 127;
  uint8_t count = 0;

  seq.addr = addr;
  seq.flags = I2C_FLAG_WRITE_READ;
  /* Select register to be read */
  regid[0] = ((uint8_t) reg);
  seq.buf[0].data = regid;
  seq.buf[0].len = 1;
  seq.buf[1].data = data;
  seq.buf[1].len = 2;

  SLEEP_SleepBlockBegin (LOWEST_ENERGY_STATE_TRANSMISSION);
  /* Do a polled transfer */
  I2C_Status = I2C_TransferInit (i2c, &seq);
  while (I2C_Status == i2cTransferInProgress)
  {
    // Sleep while waiting for an interruptLETIMER_ULFRCO_FREQ
    SLEEP_Sleep ();
    if (count == MAX_COUNT)
    {
      break;
    }
    count++;
  }

  SLEEP_SleepBlockEnd (LOWEST_ENERGY_STATE_TRANSMISSION);
  if (I2C_Status != i2cTransferDone)
  {
    return ((int) I2C_Status);
  }

  *val = (((uint16_t) (data[0])) << 8) | data[1];

  return (0);
}

int
LIGHTSENS_RegisterSet (I2C_TypeDef * i2c, uint8_t addr,
                      LIGHTSENS_Register_TypeDef reg, uint16_t val)
{
  I2C_TransferSeq_TypeDef seq;
  uint8_t data[3];
  const uint8_t MAX_COUNT = 127;
  uint8_t count = 0;

  seq.addr = addr;
  seq.flags = I2C_FLAG_WRITE;
  /* Select register to be written */
  data[0] = ((uint8_t) reg);
  seq.buf[0].data = data;
  data[1] = (uint8_t) (val >> 8);
  data[2] = (uint8_t) val;
  seq.buf[0].len = 3;

  SLEEP_SleepBlockBegin (LOWEST_ENERGY_STATE_TRANSMISSION);
  /* Do a polled transfer */
  I2C_Status = I2C_TransferInit (i2c, &seq);
  while (I2C_Status == i2cTransferInProgress)
  {
    /* Enter EM1 while waiting for I2C interrupt */
    // Sleep while waiting for an interrupt
    SLEEP_Sleep ();
    if (count == MAX_COUNT)
    {
      break;
    }
    count++;
    /* Could do a timeout function here. */
  }
  SLEEP_SleepBlockEnd (LOWEST_ENERGY_STATE_TRANSMISSION);

  return (I2C_Status);
}
