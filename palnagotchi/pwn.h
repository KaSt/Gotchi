#ifndef _PWN_H_
#define _PWN_H_

#include <stdio.h>
#include <stdlib.h>
#include "ArduinoJson.h"
#include "EEPROM.h"
#include <SPI.h>
#include <FS.h>
#include <iterator>
#include <vector>
#include <set>
#include "M5Unified.h"
#include "esp_wifi.h"
#include <esp_netif.h>
#include "esp_wifi_types.h"
#include "freertos/FreeRTOS.h"
#include "pwnagotchi.h"
#include "environment.h"
#include "config.h"
#include "db.h"

#define GRID_VERSION    "1.10.3"
#define PWNGRID_VERSION "1.8.4"

extern void drawMood(String face, String phrase, bool broken,
                      String last_friend_name, signed int rssi);

#define MAX_PKT_SAVE 800

struct BeaconEntry {
    uint8_t mac[6]{};
    uint8_t channel{0};

    bool operator<(const BeaconEntry &other) const {
        int cmp = memcmp(mac, other.mac, sizeof(mac));
        if (cmp != 0) { return cmp < 0; }
        return channel < other.channel;
    }
};

int wifi_get_channel();
void wifi_set_channel(int ch);

void initPwning();
esp_err_t pwngridAdvertise(uint8_t channel, String face);
pwngrid_peer* getPwngridPeers();
uint8_t getPwngridRunTotalPeers();
uint8_t getPwngridTotalPeers();
String getPwngridLastFriendName();
signed int getPwngridClosestRssi();
void checkPwngridGoneFriends();
uint8_t getPwngridRunPwned();
uint8_t getPwngridTotalPwned();
void performDeauthCycle();
Environment &getEnv();


#endif
