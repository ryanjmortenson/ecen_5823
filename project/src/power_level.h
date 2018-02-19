/*
 * power_level.h
 *
 *  Created on: Feb 18, 2018
 *      Author: mortzula
 */

#ifndef SRC_POWER_LEVEL_H_
#define SRC_POWER_LEVEL_H_

/*
 * @brief Gets power level based on RSSI
 * @param rssi RSSI level
 * @return power level
 */
int16_t
get_power_level (int8_t rssi);

#endif /* SRC_POWER_LEVEL_H_ */
