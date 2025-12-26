#ifndef TYPE_DEFINITIONS_H
#define TYPE_DEFINITIONS_H

#include <stdio.h>

/* 
 * CREW MEMBERS
 */

#define JOB_STRING_MAX_LENGTH 64
#define CREW_MEMBER_NAME_MAX_LENGTH 64
#define AMOUNT_OF_DIFFERENT_JOBS 2

enum job { // Try not to forget to update AMOUNT_OF_DIFFERENT_JOBS 
    ENGINEER,
    PILOT,
};

typedef struct
{
    size_t id;
    char name[CREW_MEMBER_NAME_MAX_LENGTH];
    enum job job;
} crewMember;


/* 
 * ROOMS
 */

#define ROOM_NAME_MAX_LENGTH 128
#define AMOUNT_OF_DIFFERENT_ROOM_TYPES 4

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

/* 
 * CREW ROOM LINKS
 */

typedef struct
{
    crewMember *crewMember;
    room *currentRoom; /** The room inside which the crew member is */
    room *destinationRoom; /** The room where the crew member is headed (can be NULL) */
} crewRoomLink;

/* 
 * CONFIG
 */

enum configType
{
    STRING,
    INT,
    ARRAY_OF_INT,
};

#endif // !TYPE_DEFINITIONS_H
