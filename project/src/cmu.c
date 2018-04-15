//***********************************************************************************
// Include files
//***********************************************************************************
#include "cmu.h"
#include "main.h"

//***********************************************************************************
// defined files
//***********************************************************************************

//***********************************************************************************
// global variables
//***********************************************************************************

//***********************************************************************************
// function prototypes
//***********************************************************************************
void cmu_init (void)
{
  // Use ULRFCO for timer
  CMU_ClockSelectSet (cmuClock_LFA, cmuSelect_ULFRCO);

  // Peripheral clocks enabled
  CMU_ClockEnable (cmuClock_GPIO, true);

  // Enable clock to the LETIMER
  CMU_ClockEnable (cmuClock_LETIMER0, true);

  // Enable the COREELE
  CMU_ClockEnable (cmuClock_CORELE, true);

  // Peripheral clocks enabled
  CMU_ClockEnable (cmuClock_ADC0, true);
}
