#ifndef CREW_ROOM_LINKER_MODEL_H
#define CREW_ROOM_LINKER_MODEL_H

#include <stdlib.h>
#include <stdbool.h>
#include "../core.h"
#include "type_definitions.h"

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
crewRoomLink *getCrewRoomLinkByCrewMemberId(
    crewRoomLink **crewRoomLinkerToLookIn,
    size_t crewRoomLinkerToLookInSize,
    size_t crewMemberIdToLookFor
);

#endif // !CREW_ROOM_LINKER_MODEL_H