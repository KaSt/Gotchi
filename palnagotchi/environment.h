#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "Arduino.h"
#include "M5Unified.h"
#include "WiFi.h"
#include "esp_wifi.h"
#include <set>  // ADD THIS - required for std::set

// Forward declarations of helper functions
uint64_t getMacFromPacket(const wifi_promiscuous_pkt_t* pkt);
bool isEAPOL(const wifi_promiscuous_pkt_t* pkt);
bool isCompleteHandshake(const wifi_promiscuous_pkt_t* pkt);
bool hasPMKID(const wifi_promiscuous_pkt_t* pkt);

struct Environment {
    // Observable state
    int ap_count = 0;
    int new_aps_found = 0;
    int eapol_packets = 0;
    bool got_handshake = false;
    bool got_pmkid = false;
    uint32_t last_handshake_time = 0;
    uint32_t epoch=0;
    unsigned long time_on_channel;  // Milliseconds on current channel
    unsigned long idle_time;   // Time spent in idle mode
    
    // Internal tracking
    uint32_t channel_start_time = 0;
    
    void reset() {
        new_aps_found = 0;
        ap_count = 0;
        eapol_packets = 0;
        got_handshake = false;
        got_pmkid = false;
        time_on_channel = 0;
        idle_time = 0;
        channel_start_time = millis();
    }
    
    void update() {
        time_on_channel = millis() - channel_start_time;
    }
};

#endif // ENVIRONMENT_H