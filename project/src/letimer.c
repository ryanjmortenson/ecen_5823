//***********************************************************************************
// Include files
//***********************************************************************************
#include "em_letimer.h"
#include "cmu.h"

//***********************************************************************************
// defined files
//***********************************************************************************


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
  .ufoa0          = letimerUFOAToggle,         /* PWM output on output 0 */
  .ufoa1          = letimerUFOAToggle,       /* PWM output on output 1*/
  .repMode        = letimerRepeatFree       /* Count until stopped */
  };
#else
  const LETIMER_Init_TypeDef le_init = LETIMER_INIT_DEFAULT;
#endif

//***********************************************************************************
// function prototypes
//***********************************************************************************
void letimer_init(void){
	// Enable clock to the LETIMER
	CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);
	CMU_ClockEnable(cmuClock_LETIMER0, true);
	CMU_ClockEnable(cmuClock_CORELE, true);

	// Initialize LETIMER
    LETIMER_Init(LETIMER0, &le_init);

    // Initialize the compare values
    LETIMER_CompareSet(LETIMER0, 0, 500);
    LETIMER_CompareSet(LETIMER0, 1, 400);
 }
