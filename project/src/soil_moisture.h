#ifndef SRC_SOIL_MOISTURE_H_
#define SRC_SOIL_MOISTURE_H_

#include <stdint.h>

#define SOIL_MOISTURE_POWER_PORT    gpioPortD
#define SOIL_MOISTURE_SIG_PORT      gpioPortA

#define SIGNAL_PIN            0
#define SIGNAL_PIN_DEFAULT    false

#define POWER_PIN	      11
#define POWER_PIN_DEFAULT     false

#define ADC_PRS_CH_SELECT       adcPRSSELCh0
#define ADC_INPUT0              adcPosSelAPORT3XCH8     /* PA0 */
#define ADC_SINGLE_DVL          4
#define ADC_BUFFER_SIZE         64

void soil_moisture_init ();

uint32_t get_soil_moisture();

void soil_moisture_dest();

void handle_soil_moisture_event();

#endif /* SRC_SOIL_MOISTURE_H_ */
