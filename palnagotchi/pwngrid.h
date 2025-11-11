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

extern void drawMood(String face, String phrase, bool broken);

static QueueHandle_t pktQueue = NULL;
static QueueHandle_t frQueue = NULL;

#define MAX_PKT_SAVE 800

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
