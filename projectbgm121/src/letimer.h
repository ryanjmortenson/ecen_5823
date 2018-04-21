#ifndef LETIMER_H_
#define LETIMER_H_

//***********************************************************************************
// Include files
//***********************************************************************************
#include <stdint.h>

//***********************************************************************************
// defined files
//***********************************************************************************
#include "em_cmu.h"
#include "native_gecko.h"
#include "em_core.h"


//***********************************************************************************
// global variables
//***********************************************************************************

//***********************************************************************************
// macros
//***********************************************************************************
#define LETIMER_INTERRUPTS (LETIMER_IF_COMP0 | LETIMER_IF_COMP1)
#define SAMPLE_PERIOD (1.0f) // 2 minutes
#define SENSOR_INIT_TIME (.120f) // 120 milliseconds for sensors to fully initialize
#define CALCULATE_INIT_DUTY_CYCLE(init_time) ((SAMPLE_PERIOD - init_time) / SAMPLE_PERIOD)

//***********************************************************************************
// function prototypes
//***********************************************************************************

/*!
 * @brief Initialize LETIMER with interrupt
 * @param period_sec period in seconds
 * @param duty_cycle as a fraction .10 => 10%
 * @return pass/fail based on period_sec and duty_cycle being valid
 */
bool letimer_init (float period_sec, float duty_cycle);

#endif /* LETIMER_H_ */
