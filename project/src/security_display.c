#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "graphics.h"
#include "ustimer.h"

#include "src/shared_resources.h"


void display_pin(uint32_t pin)
{
  // Buffer for holding PK print statement
  static char passbuffer[32] = {0};

  // Turn on the display which has a shared pin of PORTD15 with the I2C devices
  turn_on_shared_resources();

  // Create the string
  snprintf(passbuffer, sizeof(passbuffer), "PK: %06"PRIu32, pin);

  // Initialize the LCD screen
  GRAPHICS_Init();

  // Clear it in case of random on pixels
  GRAPHICS_Clear();

  // Put string in memory
  GRAPHICS_AppendString(passbuffer);

  // Display
  GRAPHICS_Update();

  // Zeroize the buffer for security
  memset(passbuffer, 0, sizeof(passbuffer));
}

void shutdown_display(void)
{
  // Clear the screen
  GRAPHICS_Clear();

  // Turn off enable pin
  turn_off_shared_resources();
}

void display_bond_failure()
{
  // Display bond failed
  GRAPHICS_AppendString("\nBond Failed");
  GRAPHICS_Update();

  // Only display for 1 second
  USTIMER_Init();
  USTIMER_Delay(1000000);
  USTIMER_DeInit();

  // Turn off power to display
  shutdown_display();
}
