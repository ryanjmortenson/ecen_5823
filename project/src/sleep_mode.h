#ifndef __SLEEP_MODE_H
#define __SLEEP_MODE_H

//***********************************************************************************
// Include files
//***********************************************************************************

//***********************************************************************************
// defined files
//***********************************************************************************

//***********************************************************************************
// global variables
//***********************************************************************************

//***********************************************************************************
// macros
//***********************************************************************************

// Enums for sleep state
typedef enum sleepstate
{
  EM0 = 0, EM1, EM2, EM3, EM4, MAX_SLEEP_STATE
} sleepstate_enum;

//***********************************************************************************
// function prototypes
//***********************************************************************************

/*!
 * @brief Increment sleep lowest EM (sleep) mode that can be entered
 * @param min_em_mode minimum EM (sleep) mode that can be entered
 */
void block_sleep_mode (sleepstate_enum min_em_mode);

/*!
 * @brief Decrement lowest EM (sleep) mode
 * @param min_em_mode minimum EM (sleep) mode that can be entered
 */
void unblock_sleep_mode (sleepstate_enum min_em_mode);

/*!
 * @brief Enter lowest EM mode currently possible
 */
void sleep (void);

#endif // __SLEEP_MODE_H
