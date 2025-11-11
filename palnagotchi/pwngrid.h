#include <stdio.h>
#include <stdlib.h>
#include "ArduinoJson.h"
#include "EEPROM.h"
#include <sqlite3.h>
#include <SPI.h>
#include <FS.h>
#include "SPIFFS.h"
#include "M5Unified.h"
#include "esp_wifi.h"
#include "esp_wifi_types.h"
// #include "freertos/FreeRTOS.h"
#include "pwnagotchi.h"

extern void drawMood(String face, String phrase, bool broken);

typedef struct {
  int epoch;
  String face;
  String grid_version;
  String identity;
  String name;
  int pwnd_run;
  int pwnd_tot;
  String session_id;
  int timestamp;
  int uptime;
  String version;
  signed int rssi;
  int last_ping;
  bool gone;
  int channel;
} pwngrid_peer;

typedef struct {
  uint8_t data[800]; // to be adapted to max packet length to be saved
  size_t len;
  uint8_t channel;
  char bssid[18]; // "aa:bb:cc:dd:ee:ff"
  char type[16];  // "EAPOL" / "PMKID"
  int64_t ts_ms;
} packet_item_t;

static QueueHandle_t pktQueue = NULL;
static QueueHandle_t frQueue = NULL;
static sqlite3 *db = NULL;
static const char *DB_PATH = "/spiffs/gotchi.db";

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
