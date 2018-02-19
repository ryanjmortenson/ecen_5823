#include <stdint.h>

int16_t
get_power_level (int8_t rssi)
{
  int16_t power;

  if (rssi > -35)
  {
    power = -26;
  }
  else if (rssi <= -35 && rssi > -45)
  {
    power = -20;
  }
  else if (rssi <= -45 && rssi > -55)
  {
    power = -15;
  }
  else if (rssi <= -55 && rssi > -65)
  {
    power = -5;
  }
  else if (rssi <= -65 && rssi > -75)
  {
    power = 0;
  }
  else if (rssi <= -75 && rssi > -85)
  {
    power = 50;
  }
  else
  {
    power = 80;
  }

  return (power * 10);
}

