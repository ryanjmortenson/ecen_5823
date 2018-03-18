#include <math.h>

#include "em_i2c.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_gpio.h"
#include "bsp.h"
#include "lightsens.h"
#include "sleep_mode.h"

#define LOWEST_ENERGY_STATE_TRANSMISSION (EM2)
#define LOWEST_ENERGY_STATE_I2C (EM4)

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
void LIGHTSENS_GetLux (I2C_TypeDef * i2c, float * lux)
{
  const uint8_t MAX_RETRIES = 10;
  uint8_t retries = 0;
  uint16_t val = 0;
  while ((val & (1 << 7)) == 0 && retries < 10)
  {
    LIGHTSENS_RegisterGet (I2C0, LIGHTSENS_ADDR, lightsensRegConfig, &val);
    retries++;
  }
  LIGHTSENS_RegisterGet (I2C0, LIGHTSENS_ADDR, lightsensRegResult, &val);
  *lux = CONV_SCALE_FACTOR * pow(CONV_MANTISSA, ((val & EXPONENT_MASK) >> EXPONENT_SHIFT)) * (val & FRACTIONAL_MASK);
}

void LIGHTSENS_SetContinuous (I2C_TypeDef * i2c)
{
  uint16_t config;

  // Read the config to update
  LIGHTSENS_RegisterGet(i2c, LIGHTSENS_ADDR, lightsensRegConfig, &config);

  // Turn on continuous config
  config |= CONTINUOUS_MODE;

  // Turn off 100 ms conversion
  config &= ~(1 << 11);

  // Set the config
  LIGHTSENS_RegisterSet(i2c, LIGHTSENS_ADDR, lightsensRegConfig, config);
}

int
LIGHTSENS_RegisterGet (I2C_TypeDef * i2c, uint8_t addr,
                      LIGHTSENS_Register_TypeDef reg, uint16_t * val)
{
  I2C_TransferSeq_TypeDef seq;
  uint8_t regid[1];
  uint8_t data[2];

  seq.addr = addr;
  seq.flags = I2C_FLAG_WRITE_READ;
  /* Select register to be read */
  regid[0] = ((uint8_t) reg);
  seq.buf[0].data = regid;
  seq.buf[0].len = 1;
  seq.buf[1].data = data;
  seq.buf[1].len = 2;

  block_sleep_mode (LOWEST_ENERGY_STATE_TRANSMISSION);
  /* Do a polled transfer */
  I2C_Status = I2C_TransferInit (i2c, &seq);
  while (I2C_Status == i2cTransferInProgress)
  {
    // Sleep while waiting for an interrupt
    sleep ();
  }

  unblock_sleep_mode (LOWEST_ENERGY_STATE_TRANSMISSION);
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

  seq.addr = addr;
  seq.flags = I2C_FLAG_WRITE;
  /* Select register to be written */
  data[0] = ((uint8_t) reg);
  seq.buf[0].data = data;
  data[1] = (uint8_t) (val >> 8);
  data[2] = (uint8_t) val;
  seq.buf[0].len = 3;

  /* Do a polled transfer */
  I2C_Status = I2C_TransferInit (i2c, &seq);
  while (I2C_Status == i2cTransferInProgress)
  {
    /* Enter EM1 while waiting for I2C interrupt */
    EMU_EnterEM1 ();
    /* Could do a timeout function here. */
  }

  return (I2C_Status);
}
