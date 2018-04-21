#include <stdbool.h>

#include "em_core.h"
#include "em_gpio.h"
#include "em_cmu.h"

// Is the lcd in use
static uint8_t ref_count = 0;

void
turn_on_shared_resources ()
{
  CORE_ATOMIC_SECTION(
    if (ref_count == 0)
    {
      GPIO_DriveStrengthSet (gpioPortD, gpioDriveStrengthWeakAlternateWeak);
      GPIO_PinModeSet (gpioPortD, 15, gpioModePushPull, true);
      CMU_ClockEnable (cmuClock_HFPER, true);
    }
    ref_count++;
  )
}

void
turn_off_shared_resources ()
{
  CORE_ATOMIC_SECTION(
    if (ref_count > 0)
    {
      ref_count--;
    }
    if (ref_count == 0)
    {
      GPIO_PinModeSet (gpioPortD, 15, gpioModePushPull, false);
      CMU_ClockEnable (cmuClock_HFPER, false);
    }
  )
}
