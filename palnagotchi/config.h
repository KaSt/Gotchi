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
#define SNIFFER 1
#ifdef I_CAN_BE_BAD
#define AGGRESSIVE 2
#endif

// Configuration structure
struct DeviceConfig {
  char device_name[32];
  uint8_t brightness;
  int personality;
  char* identity;
};

// Configuration functions
void initConfig();
void loadConfig();
void saveConfig();
void resetConfig();
DeviceConfig* getConfig();
String getDeviceName();
void setDeviceName(const char* name);
int getPersonality();
void setPersonality(const int personality);
String getPersonalityText();
void setIdentity(const char* identity );

#endif
