#ifndef _M5_NANO_C6_H_
#define _M5_NANO_C6_H_

#include "Arduino.h"

#define M5NANO_C6_BLUE_LED_PIN 7
#define M5NANO_C6_BTN_PIN 9
#define M5NANO_C6_IR_TX_PIN 3
#define M5NANO_C6_RGB_LED_PWR_PIN 19
#define M5NANO_C6_RGB_LED_DATA_PIN 20

namespace m5 {

class M5NanoC6 {
public:
  void begin();
  void setClock();

  void update(void);
  void toggle_led();
  void turn_led_off();
  void turn_led_on();
};
}  // namespace m5

extern m5::M5NanoC6 NanoC6;

#endif
