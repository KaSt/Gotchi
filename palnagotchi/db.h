#ifndef _DB_H_
#define _DB_H_

#include "ArduinoJson.h"
#include <LittleFS.h>
#include "structs.h"

void initDB();
bool addFriend(pwngrid_peer packet);
bool mergeFriend(const pwngrid_peer &nf);
bool addPacket(packet_item_t newFriend);
int countPackets();
int countFriends();
int countEAPOL();
int countPMKID();

#endif