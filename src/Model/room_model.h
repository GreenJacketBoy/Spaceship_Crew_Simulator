#ifndef ROOM_MODEL_H
#define ROOM_MODEL_H


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../core.h"
#include "type_definitions.h"

extern size_t currentBiggestRoomId;
extern room **roomList;
extern size_t roomListSize;

int initRoom(int amountOfRooms);
room* createRoom(
    char *name,
    enum roomType roomType,
    size_t crewCapacity,
    size_t storageCapacity,
    size_t size
);
int addAdjacentRoom(room *roomToAddTo, room *adjacentRoomToAdd);
bool isAdjacent(room *referenceRoom, room *roomToLookFor);
room *getRoomInArray(room **roomsArray, size_t roomsArraySize, size_t roomIdToLookFor);
int addRoom(room *newRoom);
int destroyRoom(size_t roomId);
room *popRoomFromRoomArray(room ***roomsArray, size_t *roomsArraySize, size_t roomIdToRemove, bool deepRemoval);
int modelEditRoom(
    room *roomToEdit,
    char *newName,
    size_t newNameLength,
    enum roomType newType,
    size_t newCrewCapacity,
    size_t newStorageCapacity,
    size_t newSize
);

#endif