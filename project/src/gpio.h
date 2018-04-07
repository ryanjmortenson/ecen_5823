#ifndef __GPIO_H
#define __GPIO_H

//***********************************************************************************
// Include files
//***********************************************************************************
#include "main.h"
#include "em_gpio.h"

//***********************************************************************************
// defined files
//***********************************************************************************

// LED0 pin is
#define LED0_port     gpioPortF
#define LED0_pin      4
#if LIGHT_LED_ON_TEMP_READ
#define LED0_default  true  // on
#else
#define LED0_default  false // on
#endif

// LED1 pin is
#define LED1_port     gpioPortF
#define LED1_pin      5
#define LED1_default  false // off

//***********************************************************************************
// global variables
//***********************************************************************************

//***********************************************************************************
// function prototypes
//***********************************************************************************

/*!
 * @brief Initialize GPIO
 */
void gpio_init (void);

#endif
