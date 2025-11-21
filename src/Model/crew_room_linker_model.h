#ifndef CREW_ROOM_LINKER_MODEL_H
#define CREW_ROOM_LINKER_MODEL_H

#include <stdlib.h>
#include <stdbool.h>
#include "../View/super_view.h"

typedef struct crewMember crewMember; // forward def from Model/crew_model.h
typedef struct room room; // forward def from Model/room_model.h

typedef struct
{
    crewMember *crewMember;
    room *currentRoom; /** The room inside which the crew member is */
    room *destinationRoom; /** The room where the crew member is headed (can be NULL) */
} crewRoomLink;

extern crewRoomLink **crewRoomLinker;
extern size_t crewRoomLinkerSize;

int addToCrewRoomLinker(
    crewRoomLink ***crewRoomLinkerToAddTo,
    size_t *crewRoomLinkerToAddToSize,
    crewRoomLink *crewRoomLinkToAdd
);
int removeFromCrewRoomLinker(
    crewRoomLink ***crewRoomLinkerToRmFrom, 
    size_t *crewRoomLinkerToRmFromSize,
    crewRoomLink *crewRoomLinkToRm
);
crewRoomLink *buildCrewRoomLink(
    crewMember *crewMember,
    room *currentRoom,
    room *destinationRoom
);
crewRoomLink *getCrewRoomLinkByCrewMember(
    crewRoomLink **crewRoomLinkerToLookIn,
    size_t crewRoomLinkerToLookInSize,
    crewMember *crewMemberToLookFor
);

#endif // !CREW_ROOM_LINKER_MODEL_H