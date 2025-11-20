#ifndef STATE_H
#define STATE_H

#include "Arduino.h"
#include "nvs_flash.h"
#include "environment.h"

// Forward declaration
struct Environment;

// Actions the agent can take
enum Action {
  STAY_CHANNEL = 0,  // Stay on current channel
  NEXT_CHANNEL = 1,  // Hop to next channel
  PREV_CHANNEL = 2,  // Hop to previous channel
  DEAUTH_MODE = 3,   // Send deauth packets (ethical monitoring only)
  IDLE_MODE = 4      // Sleep to save power
};

// State representation
struct State {
  int channel;           // 0-12 (WiFi channels 1-13)
  int ap_density;        // 0=empty, 1=low (1-2), 2=high (3+)
  int recent_success;    // 0=no, 1=yes (handshake/PMKID in last N epochs)
  int time_bucket;       // 0=night, 1=morning, 2=afternoon, 3=evening
  uint32_t epoch;        // Current training epoch (for display only)

  // Convert state to index for Q-table lookup
  int toIndex() const {
    // State space: 13 channels × 3 densities × 2 success × 4 time buckets = 312 states
    return channel * 24 + ap_density * 8 + recent_success * 4 + time_bucket;
  }

  // Create state from environment observation
  static State fromObservation(const Environment& env);
};

// Global environment accessor
Environment& getEnv();

// Helper function to get time bucket
inline int getTimeBucket() {
  // Get current hour (0-23)
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return 1;  // Default to afternoon if time not available
  }
  
  int hour = timeinfo.tm_hour;
  
  // Divide day into 4 buckets
  if (hour >= 0 && hour < 6) return 0;      // Night (00:00-06:00)
  if (hour >= 6 && hour < 12) return 1;     // Morning (06:00-12:00)
  if (hour >= 12 && hour < 18) return 2;    // Afternoon (12:00-18:00)
  return 3;                                  // Evening (18:00-24:00)
}

// Implementation of State::fromObservation
inline State State::fromObservation(const Environment& env) {
  State s;
  
  // Get current WiFi channel (you need to implement wifi_get_channel())
  extern int wifi_get_channel();
  int current_ch = wifi_get_channel();
  s.channel = (current_ch - 1) % 13;  // Normalize to 0-12
  
  // Classify AP density
  if (env.ap_count == 0) {
    s.ap_density = 0;  // Empty
  } else if (env.ap_count <= 2) {
    s.ap_density = 1;  // Low density
  } else {
    s.ap_density = 2;  // High density
  }
  
  // Recent success flag (this could be tracked over last N epochs)
  // For now, we just use current step success
  s.recent_success = (env.got_handshake || env.got_pmkid) ? 1 : 0;
  
  // Get time bucket
  s.time_bucket = getTimeBucket();
  
  s.epoch = 0;  // Will be set by agent
  
  return s;
}

void initBrain();

#endif // STATE_H