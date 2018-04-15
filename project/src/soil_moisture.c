#include "em_core.h"
#include "em_gpio.h"
#include "em_adc.h"
#include "native_gecko.h"
#include "main.h"

#include "src/soil_moisture.h"

#define MAX_CONVER_COUNT (32)

uint32_t measurement_count = 0;
uint32_t sum = 0;
extern uint32_t events;

void ADC0_IRQHandler(void)
{
  CORE_ATOMIC_IRQ_DISABLE ();

  // Disable interrupts until event can be processed
  ADC_IntDisable(ADC0, ADC_IEN_SINGLE | ADC_IEN_SINGLECMP);

  // Set soil moisture read event
  SET_EVENT(events, READ_SOIL_MOISTURE);

  // Signal events with gecko system
  gecko_external_signal (events);

  CORE_ATOMIC_IRQ_ENABLE ();
}

void soil_moisture_init (void)
{
  ADC_Init_TypeDef init = ADC_INIT_DEFAULT;
  ADC_InitSingle_TypeDef singleInit = ADC_INITSINGLE_DEFAULT;

  /* Initialize for single conversion */
  singleInit.reference = adcRefVDD;
  singleInit.posSel = ADC_INPUT0;
  singleInit.negSel = adcNegSelVSS;
  singleInit.fifoOverwrite = true;
  singleInit.acqTime = adcAcqTime16;
  ADC_InitSingle(ADC0, &singleInit);

  /* Enable single window compare */
  ADC0->SINGLECTRL |= ADC_SINGLECTRL_CMPEN;

  /* Enable ADC Interrupt when reaching DVL and window compare */
  ADC_IntEnable(ADC0, ADC_IEN_SINGLE | ADC_IEN_SINGLECMP);
  ADC_Init(ADC0, &init);

  /* Clear the FIFOs and pending interrupt */
  ADC0->SINGLEFIFOCLEAR = ADC_SINGLEFIFOCLEAR_SINGLEFIFOCLEAR;
  NVIC_ClearPendingIRQ(ADC0_IRQn);
  NVIC_EnableIRQ(ADC0_IRQn);

  // Set port D for soil moisture sensor
  GPIO_DriveStrengthSet (SOIL_MOISTURE_POWER_PORT, gpioDriveStrengthStrongAlternateStrong);
  GPIO_DriveStrengthSet (SOIL_MOISTURE_SIG_PORT, gpioDriveStrengthStrongAlternateStrong);

  // Set input and output pins for soil moisture sensor
  GPIO_PinModeSet (SOIL_MOISTURE_POWER_PORT, POWER_PIN, gpioModePushPull, true);
  GPIO_PinModeSet (SOIL_MOISTURE_SIG_PORT, SIGNAL_PIN, gpioModeInput, true);

  // Reset the sum and measurement count
  sum = measurement_count = 0;

  // Start Conversion
  ADC_Start(ADC0, adcStartSingle);
}

void soil_moisture_dest()
{
  // Set input and output pins for soil moisture sensor
  GPIO_PinModeSet (SOIL_MOISTURE_POWER_PORT, POWER_PIN, gpioModeDisabled, false);
  GPIO_PinModeSet (SOIL_MOISTURE_SIG_PORT, SIGNAL_PIN, gpioModeDisabled, false);
  ADC_Reset(ADC0);
}

void handle_soil_moisture_event()
{
  sum += ADC_DataSingleGet(ADC0);
  measurement_count++;

  if (measurement_count < MAX_CONVER_COUNT)
  {
    // Turn interrupts back on
    ADC_IntEnable(ADC0, ADC_IEN_SINGLE | ADC_IEN_SINGLECMP);

    // Restart the ADC calculation
    ADC_Start(ADC0, adcStartSingle);
  }
}

uint32_t get_soil_moisture()
{
  if (measurement_count > 0)
  {
    return sum / measurement_count;
  }
  else
  {
    return 0;
  }
}

