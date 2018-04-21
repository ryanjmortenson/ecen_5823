#include <stdint.h>
#include <native_gecko.h>

int8_t load_or_set_initial(uint16_t key, uint32_t set_val, uint32_t* load_val)
{
  int8_t res = 0;
  struct gecko_msg_flash_ps_load_rsp_t* rsp = gecko_cmd_flash_ps_load(key);

  if (rsp->result == 0x0502)
  {
      gecko_cmd_flash_ps_save(key, 4, (uint8_t *)set_val);
      res = 1;
  }
  else if (rsp->result != 0)
  {
      res = -1;
  }
  else
  {
      *load_val = *(uint32_t *)rsp->value.data;
  }
  return res;
}

int8_t load(uint16_t key, uint32_t* load_val)
{
  int8_t res = 0;
  struct gecko_msg_flash_ps_load_rsp_t* rsp = gecko_cmd_flash_ps_load(key);

  if (rsp->result != 0)
  {
      res = -1;
  }
  else
  {
      *load_val = *(uint32_t *)rsp->value.data;
  }
  return res;
}

int8_t save(uint16_t key, uint32_t save_val)
{
  int8_t res = 0;
  struct gecko_msg_flash_ps_save_rsp_t* rsp = gecko_cmd_flash_ps_save(key, 4, (uint8_t *)&save_val);

  if (rsp->result != 0)
  {
      res = -1;
  }
  return res;
}



