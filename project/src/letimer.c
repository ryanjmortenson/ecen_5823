//***********************************************************************************
// Include files
//***********************************************************************************
#include "em_letimer.h"
#include "cmu.h"

//***********************************************************************************
// defined files
//***********************************************************************************
#include "letimer.h"
#include "events.h"

//***********************************************************************************
// macros
//***********************************************************************************
#define LETIMER_MAX         (1<<16)
#define LETIMER_LFXO_FREQ   (32768)
#define LETIMER_ULFRCO_FREQ (1000)
#define TWO_TO_THE(x)       (1 << x)

//***********************************************************************************
// global variables
//***********************************************************************************

const LETIMER_Init_TypeDef le_init = {.enable = true, // Start counting when
                                                      // init completed.
  .debugRun = false,  // Counter shall not keep running during debug halt.
  .comp0Top = true, // Load COMP0 register into CNT when counter underflows.
                    // COMP0 is used as TOP
  .bufTop = false,  // Don't load COMP1 into COMP0 when REP0 reaches 0.
  .out0Pol = 0, // Leave PWM output 0 at 0
  .out1Pol = 0, // Leave PWM output 1 at 0
  .ufoa0 = letimerUFOANone, // No change on timer
  .ufoa1 = letimerUFOANone, // No change on timer
  .repMode = letimerRepeatFree  // Let run forever
};

extern uint8_t events;

//***********************************************************************************
// functions
//***********************************************************************************

void LETIMER0_IRQHandler (void)
{
  CORE_ATOMIC_IRQ_DISABLE ();

  // Clear interrupt
  LETIMER_IntClear (LETIMER0, LETIMER_INTERRUPTS);

  // Set the events
  set_events (&events);

  // Signal events with gecko system
  gecko_external_signal (events);

  CORE_ATOMIC_IRQ_ENABLE ();
}

bool letimer_init (float period_sec, float duty_cycle)
{
  bool valid = true;
  uint32_t total_cycles = 0;
  int8_t prescaler = -1;
  uint32_t power_of_two = 0;

  // Validate inputs and that the timer can be achieved
  do
  {
    // Validate the the period is not less than 1 cycle of the oscillator
    if (period_sec < (float) (1.0f / LETIMER_ULFRCO_FREQ))
    {
      valid = false;
      break;
    }

    // Try to find a pre-scaler that can accommodate period_sec
    for (uint16_t i = 0; (power_of_two = TWO_TO_THE (i)) < cmuClkDiv_512; i++)
    {
      // Calculate total cycles needed at current pre-scaler and frequency then
      // check to see if that is greater than the 65536 count
      total_cycles = (period_sec * LETIMER_ULFRCO_FREQ) / power_of_two;
      if (total_cycles < LETIMER_MAX)
      {
        // Success set the pre-scaler and break out of the loop
        prescaler = power_of_two;
        break;
      }
    }

    // If the pre-scaler wasn't set, fail and bail out
    if (prescaler == -1)
    {
      valid = false;
      break;
    }

    // If the duty cycle is out of range fail and bail out
    if (duty_cycle > 1.0f || duty_cycle < 0.0f)
    {
      valid = false;
      break;
    }
  }
  while (0);

  if (valid)
  {
    // Everything is valid, calculate the number of cycles on
    uint16_t on_cycles = total_cycles * duty_cycle;

    // Set the pre-scaler
    CMU_ClockDivSet (cmuClock_LETIMER0, prescaler);

    // Initialize LETIMER
    LETIMER_Init (LETIMER0, &le_init);

    // Initialize the compare values
    LETIMER_CompareSet (LETIMER0, 0, total_cycles);
    LETIMER_CompareSet (LETIMER0, 1, on_cycles);

    // Enable LETIMER interrupt
    LETIMER_IntEnable (LETIMER0, LETIMER_INTERRUPTS);

    /* Enable LETIMER0 interrupt vector in NVIC */
    NVIC_EnableIRQ (LETIMER0_IRQn);
  }
  return valid;
}
