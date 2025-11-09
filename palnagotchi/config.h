#ifndef CONFIG_H
#define CONFIG_H

#include "Arduino.h"
#include "EEPROM.h"

// EEPROM memory layout
#define EEPROM_SIZE 512
#define EEPROM_FRIENDS_ADDR 0
#define EEPROM_CONFIG_MAGIC_ADDR 1
#define EEPROM_CONFIG_START_ADDR 5
#define CONFIG_MAGIC_VALUE 0xAB

// Personality
#define FRIENDLY 0
#define PASSIVE 1
#define AGGRESSIVE 2

// Configuration structure
struct DeviceConfig {
  char device_name[32];
  uint8_t brightness;
  int personality;
};

// Configuration functions
void initConfig();
void loadConfig();
void saveConfig();
void resetConfig();
DeviceConfig* getConfig();
String getDeviceName();
void setDeviceName(const char* name);

#endif
