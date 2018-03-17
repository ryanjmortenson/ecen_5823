#include "em_core.h"
#include "em_gpio.h"
#include "em_adc.h"

#include "src/soil_moisture.h"

uint32_t adcBuffer[ADC_BUFFER_SIZE];
uint8_t measurement_count = 0;
uint32_t sum = 0;

void ADC0_IRQHandler(void)
{
  CORE_ATOMIC_IRQ_DISABLE ();


    /* Read SINGLEDATA will clear SINGLE IF flag */
    adcBuffer[0] = ADC_DataSingleGet(ADC0);
    sum += adcBuffer[0];
    adcBuffer[1] = ADC_DataSingleGet(ADC0);
    adcBuffer[2] = ADC_DataSingleGet(ADC0);
    adcBuffer[3] = ADC_DataSingleGet(ADC0);


#if 0
  /* SINGLECMP or SCANCMP trigger */
  /* The SCANCMP and SINGLECMP interrupt flags in ADCn_IF are not clearable */
  /* in certain scenarios (Errata ADC_E208). */
  /* Workaround is to clear CMPEN before clearing the SINGLECMP IF flag */
  /* but SINGLECTRL register can only be accessed on ADC SYNC mode. */
  /* Alternative is to disable ADC interrupt to avoid multiple SCANCMP or */
  /* SINGLECMP interrupts from the same source. */
  if ((ADC0->IF & ADC_IF_SINGLECMP) || (ADC0->IF & ADC_IF_SCANCMP))
  {
    /* Stop PRS trigger from RTCC, read interrupt flag with clear */
    NVIC_DisableIRQ(ADC0_IRQn);
  }
#endif

  measurement_count++;
  if (measurement_count == 255)
  {
    // Set input and output pins for soil moisture sensor
    GPIO_PinModeSet (SOIL_MOISTURE_POWER_PORT, POWER_PIN, gpioModeDisabled, false);
    GPIO_PinModeSet (SOIL_MOISTURE_SIG_PORT, SIGNAL_PIN, gpioModeDisabled, false);

    ADC_Reset(ADC0);
  }
  else
  {
    ADC_Start(ADC0, adcStartSingle);
  }
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
  ADC_InitSingle(ADC0, &singleInit);

  /* Enable single window compare */
  ADC0->SINGLECTRL |= ADC_SINGLECTRL_CMPEN;

  /* Set single data valid level (DVL) to trigger */
  // ADC0->SINGLECTRLX |= (ADC_SINGLE_DVL - 1) << _ADC_SINGLECTRLX_DVL_SHIFT;

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

  sum = measurement_count = 0;

  ADC_Start(ADC0, adcStartSingle);
}

uint32_t get_soil_moisture()
{
  return sum / 255;
}

