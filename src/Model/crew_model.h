#ifndef CREW_MODEL_H
#define CREW_MODEL_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../core.h"
#include "type_definitions.h"

typedef struct {
    room *roomPointer;
    bool isColored;
} coloredRoom;

int addCrewMember(crewMember *newCrewMember, crewMember ***crewList, size_t *crewListSize);
crewMember *buildCrewMember(enum job job, char *name, size_t nameLength, size_t *currentBiggestCrewMemberId);
int destroyCrewMember(size_t crewMemberId, crewMember ***crewList, size_t *crewListSize);
crewMember *getCrewMemberFromArray(size_t crewMemberId, crewMember **arrayToLookIn, size_t arraySize);
int modelEditCrewMember(crewMember *crewMemberToEdit, enum job job, char *name, size_t nameLength);
int modelMoveCrewMember(crewMember *crewMemberToMove, crewMember **crewList, size_t crewListSize, room **roomList, size_t roomListSize);
int modelCrewMemberGoTo(crewMember *crewMemberToMove, size_t roomIdNewDestination, room **roomList, size_t roomListSize);
size_t modelCountCrewInRoom(size_t roomId, crewMember **crewList, size_t crewListSize);
bool isRoomLeadingToDestination(room *initialRoom, room *destinationRoom, coloredRoom *coloredRooms, size_t roomListSize, crewMember **crewList, size_t crewListSize);
room *pathfindNextRoom(room *initialRoom, room *destinationRoom, room **roomList, size_t roomListSize, crewMember **crewList, size_t crewListSize);

#endif