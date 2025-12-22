#include "crew_model.h"

size_t currentBiggestCrewMemberId = 0;
crewMember** crewList = NULL;
size_t crewListSize = 0;


int addCrewMember(crewMember *newCrewMember)
{
    crewMember **newCrewList = malloc(sizeof(crewMember*) * (crewListSize + 1));
    if (newCrewList == NULL) 
        goto error_malloc_crewmember;

    for (size_t i = 0; i < crewListSize; i++)
    {
        newCrewList[i] = crewList[i];
    }

    newCrewList[crewListSize] = newCrewMember;
    crewListSize++;

    free(crewList);
    crewList = newCrewList;

    crewRoomLink *newCrewRoomLink = buildCrewRoomLink((crewMember *) newCrewMember, NULL, NULL);
    if (newCrewMember == NULL)
        goto error_build_linker;
    if (addToCrewRoomLinker(&crewRoomLinker, &crewRoomLinkerSize, newCrewRoomLink) != 0)
        goto error_add_to_linker;

    return 0;

error_malloc_crewmember:
    displayError("ERROR: Failed to allocate enough memory to add a crewman to the list");
    return -1;
error_build_linker:
    displayError("ERROR: Something went wrong when building the crew room linker");
    return -2;
error_add_to_linker:
    displayError("ERROR: Something went wrong when adding the room to the crew room linker");
    free(newCrewRoomLink);
    return -3;
}

crewMember *buildCrewMember(enum job job, char *name, size_t nameLength)
{
    crewMember *newCrewMember = malloc(sizeof(crewMember));
    if (newCrewMember == NULL)
    {
        return NULL;
    }

    currentBiggestCrewMemberId++;

    newCrewMember->id = currentBiggestCrewMemberId;
    strncpy(newCrewMember->name, name, CREW_MEMBER_NAME_MAX_LENGTH);
    newCrewMember->job = job;

    return newCrewMember;
}

int initCrew(int crewSize)
{
    crewMember *newCrewMember = buildCrewMember(PILOT, "James", CREW_MEMBER_NAME_MAX_LENGTH);
    if (newCrewMember == NULL)
        return -1;
    
    if (addCrewMember(newCrewMember) != 0)
        return -2;

    return 0;
}

int destroyCrewMember(size_t crewMemberId)
{
    if (crewListSize == 0)
        goto error_list_is_empty;

    crewMember **newCrewList = malloc(sizeof(crewMember*) * (crewListSize - 1));

    if (crewListSize != 1 && newCrewList == NULL)
        goto error_malloc_failure;
 
    bool crewMemberWasFound = false;
    for (size_t i = 0; i < crewListSize; i++)
    {
        if (crewMemberWasFound)
            newCrewList[i - 1] = crewList[i]; // -1 to account for the removed member
        else if (crewList[i]->id == crewMemberId)
        {
            crewMemberWasFound = true;
            crewMember *crewMemberToDestroy = crewList[i];
            crewRoomLink *crewRoomLinkToRemove = getCrewRoomLinkByCrewMember(crewRoomLinker, crewRoomLinkerSize, (crewMember *) crewMemberToDestroy);
            if (crewRoomLinkToRemove == NULL)
                goto error_link_not_found;
            if (removeFromCrewRoomLinker(&crewRoomLinker, &crewRoomLinkerSize, crewRoomLinkToRemove) != 0)
                goto error_could_not_rm_from_linker;
            free(crewMemberToDestroy);
            crewMemberToDestroy = NULL;
        }
        else if (i >= crewListSize - 1)
            goto error_crew_member_not_found;
        else
            newCrewList[i] = crewList[i]; 
    }
    
    crewListSize--;
    free(crewList);
    crewList = newCrewList;
    return 0;

error_list_is_empty:
    return -2;
error_malloc_failure:
    return -1;
error_crew_member_not_found:
    free(newCrewList);
    return -404; // ahahah very funny
error_link_not_found:
    free(newCrewList);
    return -3;
error_could_not_rm_from_linker:
    free(newCrewList);
    return -4;
}

crewMember *getCrewMemberFromArray(size_t crewMemberId, crewMember **arrayToLookIn, size_t arraySize)
{
    for (size_t i = 0; i < arraySize; i++)
    {
        if (arrayToLookIn[i]->id == crewMemberId)
            return arrayToLookIn[i];
    }

    return NULL;
}

int modelEditCrewMember(crewMember *crewMemberToEdit, enum job job, char *name, size_t nameLength)
{
    strncpy(crewMemberToEdit->name, name, nameLength);
    crewMemberToEdit->job = job;

    return 0;
}

int modelMoveCrewMember(crewMember *crewMemberToMove, size_t roomIdToMoveTo, room **roomList, size_t roomListSize)
{
    for (size_t i = 0; i < roomListSize; i++)
    {
        if (roomList[i]->id == roomIdToMoveTo)
        {
            crewRoomLink *crewRoomLinkToModify = getCrewRoomLinkByCrewMember(crewRoomLinker, crewRoomLinkerSize, crewMemberToMove);
            if (crewRoomLinkToModify == NULL)
                goto error_crew_room_link_404;
            crewRoomLinkToModify->currentRoom = roomList[i];
            return 0;
        }
    }

    displayError("The room is not in the roomArray");
    return -1;

error_crew_room_link_404:
    displayError("The crewRoomLink associated to the crewMember doesn't exist");
    return -2;
}