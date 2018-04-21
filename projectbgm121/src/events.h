#ifndef SRC_EVENTS_H_
#define SRC_EVENTS_H_

typedef enum events
{
  START_TEMP_SENSOR = 0, READ_TEMPERATURE, READ_SOIL_MOISTURE
} events_t;

#define CREATE_EVENT(event) (1 << event)
#define SET_EVENT(event, event_to_set) (event |= CREATE_EVENT(event_to_set))

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
