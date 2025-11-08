#ifndef CREW_VIEW_H
#define CREW_VIEW_H

#include <stdio.h>
#include <string.h>
#include "../Model/room_model.h"
#include "colors.h"
#include "super_view.h"

#define ROOM_TYPE_STRING_MAX_LENGTH 64

int listRooms();
int printRoomWithPrefix(char *prefix, room *room);
int getRoomTypeToString(enum roomType roomType, char *roomTypeStringBuffer, size_t roomTypeStringMaxLength);
int promptRoomAttributes(
    char *name,
    size_t nameLength,
    enum roomType *roomType,
    size_t *crewCapacity,
    size_t *storageCapacity,
    size_t *size
);
int viewRoomRm(size_t *roomIdToRemove);

#endif