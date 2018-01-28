//***********************************************************************************
// Include files
//***********************************************************************************
#include "em_letimer.h"
#include "cmu.h"
#include "sleep_mode.h"

//***********************************************************************************
// defined files
//***********************************************************************************
#include "letimer.h"

//***********************************************************************************
// macros
//***********************************************************************************

#define LETIMER_OSC_FREQ (1000)
#define LETIMER_PRESCALER (1)

//***********************************************************************************
// global variables
//***********************************************************************************

#if 1
  const LETIMER_Init_TypeDef le_init =
  {
  .enable         = true,                   /* Start counting when init completed. */
  .debugRun       = false,                  /* Counter shall not keep running during debug halt. */
  .comp0Top       = true,                   /* Load COMP0 register into CNT when counter underflows. COMP0 is used as TOP */
  .bufTop         = false,                  /* Don't load COMP1 into COMP0 when REP0 reaches 0. */
  .out0Pol        = 0,                      /* Idle value for output 0. */
  .out1Pol        = 0,                      /* Idle value for output 1. */
  .ufoa0          = letimerUFOANone,        /* PWM output on output 0 */
  .ufoa1          = letimerUFOANone,        /* PWM output on output 1*/
  .repMode        = letimerRepeatFree    /* Count until stopped */
  };
#else
  const LETIMER_Init_TypeDef le_init = LETIMER_INIT_DEFAULT;
#endif

//***********************************************************************************
// functions
//***********************************************************************************

bool letimer_init(float period_sec, float duty_cycle)
{
	bool valid = true;
	float max_seconds = ((1<<16) * LETIMER_PRESCALER) / LETIMER_OSC_FREQ;
	do
	{
		if (max_seconds < period_sec)
		{
			valid = false;
			break;
		}

		if (duty_cycle > 1.0f)
		{
			valid = false;
			break;
		}
	} while(0);

	if (valid)
	{
		uint16_t period_count = (period_sec * LETIMER_OSC_FREQ) * LETIMER_PRESCALER;
		uint16_t on_count = period_count * duty_cycle;

		LETIMER_RepeatSet(LETIMER0, 0, 1);
		LETIMER_RepeatSet(LETIMER0, 1, 1);

		// Initialize LETIMER
		LETIMER_Init(LETIMER0, &le_init);

		// Initialize the compare values
		LETIMER_CompareSet(LETIMER0, 0, period_count);
		LETIMER_CompareSet(LETIMER0, 1, on_count);

		// Enable LETIMER interrupt
		LETIMER_IntEnable(LETIMER0, LETIMER_INTERRUPTS);

		/* Enable LETIMER0 interrupt vector in NVIC*/
		NVIC_EnableIRQ(LETIMER0_IRQn);
	}
	return valid;
 }
