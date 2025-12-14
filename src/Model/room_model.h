#ifndef ROOM_MODEL_H
#define ROOM_MODEL_H

#define ROOM_NAME_MAX_LENGTH 128
#define AMOUNT_OF_DIFFERENT_ROOM_TYPES 4

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../core.h"

enum roomType { // Try not to forget to update AMOUNT_OF_DIFFERENT_ROOM_TYPES
    CORRIDOR,
    MEDBAY,
    QUARTER,
    BRIDGE,
};

typedef struct room
{
    size_t id; /* The unique identifier of the room */
    char name[ROOM_NAME_MAX_LENGTH];
    enum roomType roomType;
    // Crew capacity is different from Storage capacity to keep the ship clean
    size_t crewCapacity; /** Amount of Crew Members a room can hold at once */
    size_t storageCapacity; /** Amount of storage units (1 square meter) the room can hold */
    size_t size; /** in square meters */
    struct room **adjacentRoomsArray;
    size_t adjacentRoomsArraySize;
} room;

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