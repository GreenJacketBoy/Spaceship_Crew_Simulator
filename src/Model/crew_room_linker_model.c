#include "crew_room_linker_model.h"

crewRoomLink **crewRoomLinker = NULL;
size_t crewRoomLinkerSize = 0;

int addToCrewRoomLinker(
    crewRoomLink ***crewRoomLinkerToAddTo,
    size_t *crewRoomLinkerToAddToSize,
    crewRoomLink *crewRoomLinkToAdd
)
{
    crewRoomLink **newCrewRoomLinker = malloc(sizeof(crewRoomLink*) * (*crewRoomLinkerToAddToSize + 1));
    if (newCrewRoomLinker == NULL) 
        goto error_malloc_failure;

    for (size_t i = 0; i < *crewRoomLinkerToAddToSize; i++)
    {
        newCrewRoomLinker[i] = (*crewRoomLinkerToAddTo)[i];
    }

    newCrewRoomLinker[*crewRoomLinkerToAddToSize] = crewRoomLinkToAdd;
    (*crewRoomLinkerToAddToSize)++;

    free(*crewRoomLinkerToAddTo);

    *crewRoomLinkerToAddTo = newCrewRoomLinker;
    return 0;

error_malloc_failure:
    displayError("Failed to allocate enough memory to create a crew-room link");
    return -1;
}

int removeFromCrewRoomLinker(
    crewRoomLink ***crewRoomLinkerToRmFrom, 
    size_t *crewRoomLinkerToRmFromSize,
    crewRoomLink *crewRoomLinkToRm
)
{
    crewRoomLink **newCrewRoomLinker = malloc(sizeof(crewRoomLink*) * (*crewRoomLinkerToRmFromSize + 1));
    if (newCrewRoomLinker == NULL && crewRoomLinkerToRmFromSize - 1 > 0) 
        goto error_malloc_failure;

    bool crewRoomLinkWasFound = false;
    for (size_t i = 0; i < *crewRoomLinkerToRmFromSize; i++)
    {
        if (crewRoomLinkWasFound)
            newCrewRoomLinker[i - 1] = (*crewRoomLinkerToRmFrom)[i]; // -1 to account for the removed room
        else if ((*crewRoomLinkerToRmFrom)[i] == crewRoomLinkToRm)
            crewRoomLinkWasFound = true;
        else if (i >= *crewRoomLinkerToRmFromSize - 1)
            goto not_found;
        else
            newCrewRoomLinker[i] = (*crewRoomLinkerToRmFrom)[i]; 
    }

    newCrewRoomLinker[*crewRoomLinkerToRmFromSize] = crewRoomLinkToRm;
    (*crewRoomLinkerToRmFromSize)--;

    free(*crewRoomLinkerToRmFrom);

    *crewRoomLinkerToRmFrom = newCrewRoomLinker;
    return 0;

error_malloc_failure:
    displayError("Failed to allocate enough memory to pop a crew-room link");
    return -1;
not_found:
    displayError("Crew-room link could not be found");
    return -2;
}

crewRoomLink *buildCrewRoomLink(
    crewMember_FromCRLinker *crewMember,
    room_FromCRLinker *currentRoom,
    room_FromCRLinker *destinationRoom
)
{
    crewRoomLink *newCrewRoomLink = malloc(sizeof(crewRoomLink));
    if (newCrewRoomLink == NULL)
        goto error_malloc_failure;
    newCrewRoomLink->crewMember = crewMember;
    newCrewRoomLink->currentRoom = currentRoom;
    newCrewRoomLink->destinationRoom = destinationRoom;

    return newCrewRoomLink;

error_malloc_failure:
    displayError("Failed to allocate enough memory to create a crew-room link");
    return NULL;
}

crewRoomLink *getCrewRoomLinkByCrewMember(
    crewRoomLink **crewRoomLinkerToLookIn,
    size_t crewRoomLinkerToLookInSize,
    crewMember_FromCRLinker *crewMemberToLookFor
)
{
    for (size_t i = 0; i < crewRoomLinkerToLookInSize; i++)
    {
        if (crewRoomLinkerToLookIn[i]->crewMember == crewMemberToLookFor)
            return crewRoomLinkerToLookIn[i];
    }

    return NULL;
}