#ifndef ROOM_MODEL_H
#define ROOM_MODEL_H

#define ROOM_NAME_SIZE 128

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

enum roomType {
    CORRIDOR,
    MEDBAY,
    QUARTER,
    BRIDGE,
};

typedef struct room
{
    size_t id; /* The unique identifier of the room */
    char name[ROOM_NAME_SIZE];
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
int addRoom(room *newRoom);

#endif