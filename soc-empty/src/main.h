#ifndef __MAIN_H
#define __MAIN_H

//***********************************************************************************
// Include files
//***********************************************************************************

#include <stdint.h>
#include <stdbool.h>

//***********************************************************************************
// defined files
//***********************************************************************************

//***********************************************************************************
// global variables
//***********************************************************************************

//***********************************************************************************
// typedefs
//***********************************************************************************

typedef enum events
{
  START_TEMP_SENSOR = 0, READ_TEMPERATURE
} events_t;

//***********************************************************************************
// macros
//***********************************************************************************

#define LIGHT_LED_ON_TEMP_READ (0)
#define CREATE_EVENT(event) (1 << event)
#define SET_EVENT(event, event_to_set) (event |= CREATE_EVENT(event_to_set))
#define MINIMUM_TEMP (15.0f)

// EM Can't enter
#define EM_CANT_ENTER (EM3)

// LETIMER Period
#define PERIOD (3.00f)

// LETIMER Duty Cycle
#define DUTY_CYCLE (0.025f)

//***********************************************************************************
// function prototypes
//***********************************************************************************

#endif