#ifndef STATE_H
#define STATE_H

#include "Arduino.h"
#include "M5Unified.h"
#include "WiFi.h"
#include "esp_wifi.h"
#include "environment.h"

// Forward declaration
int getHour();

// State discretization
struct State {
    uint8_t channel;          // 0-12 (channels 1-13)
    uint8_t ap_density;       // 0=none, 1=few(1-3), 2=many(4+)
    uint8_t recent_success;   // 0=no recent handshake, 1=got one recently
    uint8_t time_bucket;      // 0=morning, 1=afternoon, 2=evening, 3=night
    uint32_t epoch;
    
    int toIndex() const {
        // Pack into single index: total = 13 * 3 * 2 * 4 = 312 states
        return channel + 
               13 * ap_density + 
               (13 * 3) * recent_success +
               (13 * 3 * 2) * time_bucket;
    }
    
    static State fromObservation(const Environment& env) {
        State s;
        
        s.epoch = env.epoch;

        // Fix: esp_wifi_get_channel requires two pointer arguments
        uint8_t primary;
        wifi_second_chan_t second;
        esp_wifi_get_channel(&primary, &second);
        s.channel = primary - 1;  // 0-12

        //Serial.printf("From observation we are on channel: %d\n", s.channel);
        
        // AP density from recent scan
        if (env.ap_count == 0) s.ap_density = 0;
        else if (env.ap_count <= 3) s.ap_density = 1;
        else s.ap_density = 2;
        
        // Recent success (last 30 seconds)
        s.recent_success = (millis() - env.last_handshake_time < 30000) ? 1 : 0;
        
        // Time of day (if RTC available)
        int hour = getHour();
        if (hour < 6) s.time_bucket = 3;      // night
        else if (hour < 12) s.time_bucket = 0; // morning
        else if (hour < 18) s.time_bucket = 1; // afternoon
        else s.time_bucket = 2;                // evening
        
        return s;
    }
};

// Actions
enum Action {
    STAY_CHANNEL = 0,      // Keep monitoring current channel
    NEXT_CHANNEL = 1,      // Hop to next channel
    PREV_CHANNEL = 2,      // Hop to previous channel
    DEAUTH_MODE = 3,       // Aggressive: send deauth + monitor
    IDLE_MODE = 4,         // Power save mode
    NUM_ACTIONS = 5
};

#endif // STATE_H