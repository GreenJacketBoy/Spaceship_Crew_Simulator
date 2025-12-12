#ifndef CREW_ROOM_LINKER_MODEL_H
#define CREW_ROOM_LINKER_MODEL_H

#include <stdlib.h>
#include <stdbool.h>
#include "../View/super_view.h"

// *_FromCRLinker represents * from other models, their name is different to avoid
// redifining errors when including them. This does mean however that I can't use
// attributes from * in the methods of this model
typedef struct crewMember_FromCRLinker crewMember_FromCRLinker;
typedef struct room_FromCRLinker room_FromCRLinker;

typedef struct
{
    crewMember_FromCRLinker *crewMember;
    room_FromCRLinker *currentRoom; /** The room inside which the crew member is */
    room_FromCRLinker *destinationRoom; /** The room where the crew member is headed (can be NULL) */
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
    crewMember_FromCRLinker *crewMember,
    room_FromCRLinker *currentRoom,
    room_FromCRLinker *destinationRoom
);
crewRoomLink *getCrewRoomLinkByCrewMember(
    crewRoomLink **crewRoomLinkerToLookIn,
    size_t crewRoomLinkerToLookInSize,
    crewMember_FromCRLinker *crewMemberToLookFor
);

#endif // !CREW_ROOM_LINKER_MODEL_H