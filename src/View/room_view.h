#ifndef CREW_VIEW_H
#define CREW_VIEW_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../Model/type_definitions.h"
#include "colors.h"
#include "super_view.h"
#include "../core.h"

#define ROOM_TYPE_STRING_MAX_LENGTH 64

int listRooms(room **roomList, size_t roomListSize);
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
int viewRoomEdit(
    room *roomToEdit,
    char *newName,
    size_t newNameLength,
    enum roomType *newType,
    size_t *newCrewCapacity,
    size_t *newStorageCapacity,
    size_t *newSize
);
enum roomType promptForRoomType(char *promptMessage);
int viewShowRoom(room *roomToShow, crewRoomLink **crewRoomLinker, size_t crewRoomLinkerSize);

#endif