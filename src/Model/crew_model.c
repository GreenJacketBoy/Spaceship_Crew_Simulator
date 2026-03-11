#include "crew_model.h"

int addCrewMember(crewMember *newCrewMember, crewMember ***crewList, size_t *crewListSize)
{
    crewMember **newCrewList = malloc(sizeof(crewMember*) * (*crewListSize + 1));
    if (newCrewList == NULL) 
        goto error_malloc_crewmember;

    for (size_t i = 0; i < *crewListSize; i++)
    {
        newCrewList[i] = (*crewList)[i];
    }

    newCrewList[*crewListSize] = newCrewMember;
    (*crewListSize)++;

    free(*crewList);
    *crewList = newCrewList;

    if (newCrewMember == NULL)
        goto error_build_linker;

    return 0;

error_malloc_crewmember:
    displayError("ERROR: Failed to allocate enough memory to add a crewman to the list");
    return -1;
error_build_linker:
    displayError("ERROR: Something went wrong when building the crew room linker");
    return -2;
}

crewMember *buildCrewMember(enum job job, char *name, size_t nameLength, size_t *currentBiggestCrewMemberId)
{
    crewMember *newCrewMember = malloc(sizeof(crewMember));
    if (newCrewMember == NULL)
    {
        return NULL;
    }

    (*currentBiggestCrewMemberId)++;

    newCrewMember->id = *currentBiggestCrewMemberId;
    strncpy(newCrewMember->name, name, CREW_MEMBER_NAME_MAX_LENGTH);
    newCrewMember->job = job;

    return newCrewMember;
}

int destroyCrewMember(size_t crewMemberId, crewMember ***crewList, size_t *crewListSize)
{
    if (*crewListSize == 0)
        goto error_list_is_empty;

    crewMember **newCrewList = malloc(sizeof(crewMember*) * (*crewListSize - 1));

    if (*crewListSize != 1 && newCrewList == NULL)
        goto error_malloc_failure;
 
    bool crewMemberWasFound = false;
    for (size_t i = 0; i < *crewListSize; i++)
    {
        if (crewMemberWasFound)
            newCrewList[i - 1] = (*crewList)[i]; // -1 to account for the removed member
        else if ((*crewList)[i]->id == crewMemberId)
        {
            crewMemberWasFound = true;
            crewMember *crewMemberToDestroy = (*crewList)[i];
            free(crewMemberToDestroy);
            crewMemberToDestroy = NULL;
        }
        else if (i >= (*crewListSize) - 1)
            goto error_crew_member_not_found;
        else
            newCrewList[i] = (*crewList)[i]; 
    }
    
    (*crewListSize)--;
    free(*crewList);
    *crewList = newCrewList;
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
            crewMemberToMove->currentRoom = roomList[i];
            return 0;
        }
    }

    displayError("The room is not in the roomArray");
    return -1;

error_crew_room_link_404:
    displayError("The crewRoomLink associated to the crewMember doesn't exist");
    return -2;
}

int modelCrewMemberGoTo(crewMember *crewMemberToMove, size_t roomIdNewDestination, room **roomList, size_t roomListSize)
{
    for (size_t i = 0; i < roomListSize; i++)
    {
        if (roomList[i]->id == roomIdNewDestination)
        {
            crewMemberToMove->destinationRoom = roomList[i];
            return 0;
        }
    }

    displayError("The room is not in the roomArray");
    return -1;

error_crew_room_link_404:
    displayError("The crewRoomLink associated to the crewMember doesn't exist");
    return -2;
}