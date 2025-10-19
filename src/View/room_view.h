#ifndef CREW_VIEW_H
#define CREW_VIEW_H

#include <stdio.h>
#include <string.h>
#include "../Model/room_model.h"

#define ROOM_TYPE_STRING_MAX_LENGTH 64

int listRooms();
int printRoomWithPrefix(char *prefix, room *room);
int getRoomTypeToString(enum roomType roomType, char *roomTypeStringBuffer, size_t roomTypeStringMaxLength);

#endif