#include <stdio.h>
#include <stdlib.h>
#include "ArduinoJson.h"
#include "EEPROM.h"
#include <SPI.h>
#include <FS.h>
#include "M5Unified.h"
#include "esp_wifi.h"
#include "esp_wifi_types.h"
// #include "freertos/FreeRTOS.h"
#include "pwnagotchi.h"
#include "db.h"

#define GRID_VERSION    "1.10.3"
#define PWNGRID_VERSION "1.8.4"

extern void drawMood(String face, String phrase, bool broken,
                      String last_friend_name, signed int rssi);

static QueueHandle_t pktQueue = NULL;
static QueueHandle_t frQueue = NULL;

#define MAX_PKT_SAVE 800

static const char* ADJ[] PROGMEM = {
  "brisk","calm","cheeky","clever","daring","eager","fuzzy","gentle","merry","nimble",
  "peppy","quirky","sly","spry","steady","swift","tidy","witty","zesty","zen"
};
static const char* ANM[] PROGMEM = {
  "otter","badger","fox","lynx","marten","panda","yak","koala","civet","ibis",
  "gecko","lemur","heron","tahr","fossa","saola","quokka","magpie","oriole","bongo"
};

void initPwngrid();
esp_err_t pwngridAdvertise(uint8_t channel, String face);
pwngrid_peer* getPwngridPeers();
uint8_t getPwngridRunTotalPeers();
uint8_t getPwngridTotalPeers();
String getPwngridLastFriendName();
signed int getPwngridClosestRssi();
void checkPwngridGoneFriends();
uint8_t getPwngridRunPwned();
uint8_t getPwngridTotalPwned();
int getPwngridChannel();
