#ifndef SRC_PERSISTENT_DATA_H_
#define SRC_PERSISTENT_DATA_H_
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
// #define ERASE_ALL_PERSISTENT_DATA
#define CONNECTION_COUNT_KEY (0x4000)
#define MEASUREMENT_COUNT_KEY (0x4001)

//***********************************************************************************
// function prototypes
//***********************************************************************************

/*!
 * @brief Attempt to load persistent data value, if not available set to set_val
 * @param key of persistent data
 * @param set_val value to set if key isn't available
 * @param load_val value if key is available
 *
 * @return pass/fail loading or setting value
 */
int8_t load_or_set_initial(uint16_t key, uint32_t set_val, uint32_t* load_val);

/*!
 * @brief Load value from persistent data
 * @param key of persistent data
 * @param load_val value that was loaded
 *
 * @return pass/fail loading value
 */
int8_t load(uint16_t key, uint32_t* load_val);

/*!
 * @brief Save value to persistent data
 * @param key of persistent data
 * @param save value to save
 *
 * @return pass/fail saving value
 */
int8_t save(uint16_t key, uint32_t save_val);

#endif /* SRC_PERSISTENT_DATA_H_ */
