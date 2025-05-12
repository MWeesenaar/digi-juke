#include <ArduinoOTA.h>

#include "M5NanoC6.h"


using namespace m5;

M5NanoC6 NanoC6;

void M5NanoC6::begin() {
  Serial.begin(115200);
  Serial.println("\n\n\nBooting M5NanoC6 with RFID2 module");
  pinMode(M5NANO_C6_BTN_PIN, INPUT);
  pinMode(M5NANO_C6_BLUE_LED_PIN, OUTPUT);
}

void M5NanoC6::setClock() {
  configTime(0, 0, "pool.ntp.org");

  Serial.print(F("Waiting for NTP time sync: "));
  time_t nowSecs = time(nullptr);
  while (nowSecs < 8 * 3600 * 2) {
    delay(500);
    Serial.print(F("."));
    yield();
    nowSecs = time(nullptr);
  }

  Serial.println();
  struct tm timeinfo;
  gmtime_r(&nowSecs, &timeinfo);
  Serial.print(F("Current time: "));
  Serial.print(asctime(&timeinfo));
}


void M5NanoC6::turn_led_on() {
  digitalWrite(M5NANO_C6_BLUE_LED_PIN, HIGH);
}

void M5NanoC6::turn_led_off() {
  digitalWrite(M5NANO_C6_BLUE_LED_PIN, LOW);
}

void M5NanoC6::toggle_led() {
  static bool ledState = false;  // Tracks the current state of the LED

  Serial.println("Current status of the LED is: " + String(ledState ? "ON" : "OFF"));
  ledState = !ledState;  // Toggle the state
  digitalWrite(M5NANO_C6_BLUE_LED_PIN, ledState ? HIGH : LOW);
}

