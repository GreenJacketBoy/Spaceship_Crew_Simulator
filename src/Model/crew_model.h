#ifndef CREW_MODEL_H
#define CREW_MODEL_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "crew_room_linker_model.h"
#include "../core.h"
#include "type_definitions.h"

extern size_t currentBiggestCrewMemberId;
extern crewMember** crewList;
extern size_t crewListSize;

int initCrew(int crewSize);
int addCrewMember(crewMember *newCrewMember);
crewMember *buildCrewMember(enum job job, char *name, size_t nameLength);
int destroyCrewMember(size_t crewMemberId);
crewMember *getCrewMemberFromArray(size_t crewMemberId, crewMember **arrayToLookIn, size_t arraySize);
int modelEditCrewMember(crewMember *crewMemberToEdit, enum job job, char *name, size_t nameLength);
int modelMoveCrewMember(crewMember *crewMemberToMove, size_t roomIdToMoveTo, room **roomList, size_t roomListSize);
int modelCrewMemberGoTo(crewMember *crewMemberToMove, size_t roomIdNewDestination, room **roomList, size_t roomListSize);

#endif