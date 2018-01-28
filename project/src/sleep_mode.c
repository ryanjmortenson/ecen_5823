//***********************************************************************************
// Include files
//***********************************************************************************
#include <stdint.h>
#include "em_emu.h"
#include "em_core.h"
#include "sleep_mode.h"

//***********************************************************************************
// defined files
//***********************************************************************************

//***********************************************************************************
// global variables
//***********************************************************************************

// Array to track EM levels peripherals can enter
static uint8_t sleep_block_counter[MAX_SLEEP_STATE] = { 0 };

//***********************************************************************************
// macros
//***********************************************************************************

//***********************************************************************************
// functions
//***********************************************************************************

// Public Functions see .h for description
void block_sleep_mode(sleepstate_enum minimumMode) {
  CORE_ATOMIC_IRQ_DISABLE();
  sleep_block_counter[minimumMode]++;
  CORE_ATOMIC_IRQ_ENABLE();
}

void unblock_sleep_mode(sleepstate_enum minimumMode) {
  CORE_ATOMIC_IRQ_DISABLE();
  if (sleep_block_counter[minimumMode] > 0) {
    sleep_block_counter[minimumMode]--;
  }
  CORE_ATOMIC_IRQ_ENABLE();
}

void sleep(void) {
  if (sleep_block_counter[EM0] > 0) {
    return;
  } else if (sleep_block_counter[EM1] > 0) {
    return;
  } else if (sleep_block_counter[EM2] > 0) {
    EMU_EnterEM1();
  } else if (sleep_block_counter[EM3] > 0) {
    EMU_EnterEM2(true);
  } else {
    EMU_EnterEM3(true);
  }
  return;
}
