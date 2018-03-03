/*
 * events.h
 *
 *  Created on: Feb 18, 2018
 *      Author: mortzula
 */

#ifndef SRC_EVENTS_H_
#define SRC_EVENTS_H_

/*
 * @brief Handle events sent in events flag
 * @param events set of events to handle
 */
void handle_events (uint8_t * events);

/*
 * @brief Set events
 * @param events variable to set events
 */
void set_events (uint8_t * events);

#endif /* SRC_EVENTS_H_ */
