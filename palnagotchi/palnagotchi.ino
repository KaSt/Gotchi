#ifdef ARDUINO_M5STACK_CARDPUTER
  #include "M5Cardputer.h"
#endif

#include "M5Unified.h"
#include "device_state.h"
#include "config.h"
#include "ap_config.h"
#include "ui.h"

uint8_t state;

void initM5() {
  auto cfg = M5.config();
  M5.begin(cfg);
  M5.Display.begin();
  Serial.println("startup...");

  #ifdef ARDUINO_M5STACK_CARDPUTER
    M5Cardputer.begin(cfg);
    M5Cardputer.Keyboard.begin();
  #endif
}

void setup() {
  initM5();
  initConfig();
  initAPConfig();
  initMood();
  initPwngrid();
  initUi();
  state = STATE_INIT;
}

uint8_t current_channel = 1;
uint32_t last_mood_switch = 10001;

void wakeUp() {
  for (uint8_t i = 0; i < 3; i++) {
    setMood(i);
    updateUi();
    delay(1250);
  }
}

void advertise(uint8_t channel) {
  Serial.println("Advertise...");
  uint32_t elapsed = millis() - last_mood_switch;
  if (elapsed > 50000) {
    setMood(random(2, 21));
    last_mood_switch = millis();
  }

  esp_err_t result = pwngridAdvertise(channel, getCurrentMoodFace());

  if (result == ESP_ERR_WIFI_IF) {
    setMood(MOOD_BROKEN, "", "Error: invalid interface", true);
    state = STATE_HALT;
  } else if (result == ESP_ERR_INVALID_ARG) {
    setMood(MOOD_BROKEN, "", "Error: invalid argument", true);
    state = STATE_HALT;
  } else if (result != ESP_OK) {
    setMood(MOOD_BROKEN, "", "Error: unknown", true);
    state = STATE_HALT;
  }
  Serial.println("Advertise Done.");
}

void loop() {
  M5.update();
  #ifdef ARDUINO_M5STACK_CARDPUTER
    M5Cardputer.update();
  #endif

  if (state == STATE_HALT) {
    return;
  }

  if (state == STATE_INIT) {
    wakeUp();
    state = STATE_WAKE;
  }

  if (state == STATE_AP_CONFIG) {
    handleAPConfig();
    // Check if AP mode timed out
    if (!isAPModeActive()) {
      exitAPConfigMode();
    }
    updateUi(false);
    return;
  }

  if (state == STATE_WAKE) {
    checkPwngridGoneFriends();
    advertise(current_channel++);
    if (current_channel == 15) {
      current_channel = 1;
    }
  }

  updateUi(true);
  Serial.println("Done loop");
}

void enterAPConfigMode() {
  state = STATE_AP_CONFIG;
  startAPMode();
}

void exitAPConfigMode() {
  stopAPMode();
  // Reinitialize WiFi for normal operation
  initPwngrid();
  state = STATE_WAKE;
}

uint8_t getDeviceState() {
  return state;
}
