//***********************************************************************************
// Include files
//***********************************************************************************
#include "cmu.h"
#include "main.h"
#include "sleep_mode.h"

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
  // Select the ULRFCO for the LFA branch
  if (EM_CANT_ENTER > EM3)
  {
    CMU_ClockSelectSet (cmuClock_LFA, cmuSelect_ULFRCO);
  }
  else
  {
    CMU_ClockSelectSet (cmuClock_LFA, cmuSelect_LFXO);
  }

  // Peripheral clocks enabled
  CMU_ClockEnable (cmuClock_GPIO, true);

  // Enable clock to the LETIMER
  CMU_ClockEnable (cmuClock_LETIMER0, true);

  // Enable the COREELE
  CMU_ClockEnable (cmuClock_CORELE, true);

  // Peripheral clocks enabled
  CMU_ClockEnable (cmuClock_ADC0, true);
}
