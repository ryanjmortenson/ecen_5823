#ifndef LIGHT_SENS_H_
#define LIGHT_SENS_H_


#include "em_device.h"

/*******************************************************************************
 *******************************   DEFINES   ***********************************
 ******************************************************************************/

/** I2C device address for ambient light sensor */
#define LIGHTSENS_ADDR      (0x44 << 1)
#define CONTINUOUS_MODE ((1<<10)|(1<<9))
#define SINGLE_SHOT (1<<9)


/*******************************************************************************
 ********************************   ENUMS   ************************************
 ******************************************************************************/

/** Available registers in OPT3001 sensor device */
typedef enum
{
  lightsensRegResult = 0, /**< Ambient light conversion register (read-only) */
  lightsensRegConfig = 1, /**< Configuration register */
  lightsensRegLowLimit = 2, /**< Low limit register */
  lightsensRegHighLimit = 3 /**< High limit shutdown register */
} LIGHTSENS_Register_TypeDef;

/*******************************************************************************
 *******************************   STRUCTS   ***********************************
 ******************************************************************************/

/*******************************************************************************
 *****************************   PROTOTYPES   **********************************
 ******************************************************************************/

int LIGHTSENS_GetLux (I2C_TypeDef * i2c, float * lux);

int LIGHTSENS_SetSingleShot (I2C_TypeDef * i2c);

int
LIGHTSENS_RegisterGet (I2C_TypeDef * i2c, uint8_t addr,
                      LIGHTSENS_Register_TypeDef reg, uint16_t * val);
int
LIGHTSENS_RegisterSet (I2C_TypeDef * i2c, uint8_t addr,
                      LIGHTSENS_Register_TypeDef reg, uint16_t val);


#endif /* LIGHT_SENS_H_ */
