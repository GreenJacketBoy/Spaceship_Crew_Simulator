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

int modelMoveCrewMember(crewMember *crewMemberToMove, crewMember **crewList, size_t crewListSize, room **roomList, size_t roomListSize)
{
    room *nextRoom = pathfindNextRoom(crewMemberToMove->currentRoom, crewMemberToMove->destinationRoom, roomList, roomListSize, crewList, crewListSize);
    if (nextRoom == NULL)
        return -1;

    crewMemberToMove->currentRoom = nextRoom;
    if (crewMemberToMove->currentRoom == crewMemberToMove->destinationRoom)
        crewMemberToMove->destinationRoom = NULL;
    return 0;
}

size_t modelCountCrewInRoom(size_t roomId, crewMember **crewList, size_t crewListSize)
{
    size_t crewCount = 0;
    for (size_t i = 0; i < crewListSize; i++)
        if (crewList[i]->currentRoom != NULL && crewList[i]->currentRoom->id == roomId)
            crewCount++;

    return crewCount;
}

room *pathfindNextRoom(room *initialRoom, room *destinationRoom, room **roomList, size_t roomListSize, crewMember **crewList, size_t crewListSize)
{
    room *nextRoom = NULL;
    coloredRoom *coloredRooms = NULL;

    if (destinationRoom == NULL)
        goto error_no_destination;
    if (initialRoom == NULL)
        return destinationRoom; // If they are floating in space, let's say they can get back inside in any room.
    if (initialRoom->adjacentRoomsArraySize == 0)
        goto error_unreachable_destination;

    coloredRooms = malloc(sizeof(coloredRoom) * roomListSize);
    for (size_t i = 0; i < roomListSize; i++)
    {
        coloredRooms[i].roomPointer = roomList[i];
        coloredRooms[i].isColored = coloredRooms[i].roomPointer == initialRoom ? true : false;
    }

    for (size_t i = 0; i < initialRoom->adjacentRoomsArraySize; i++)
    {
        if (isRoomLeadingToDestination(initialRoom->adjacentRoomsArray[i], destinationRoom, coloredRooms, roomListSize, crewList, crewListSize))
        {
            nextRoom = initialRoom->adjacentRoomsArray[i];
            goto free_and_return;
        }
    }

    goto error_unreachable_destination;

error_no_destination:
    displayError("The crew member does not have a destination set");
    goto free_and_return;
error_unreachable_destination:
    displayError("Destination cannot be reached (no path or the room is full)");
    goto free_and_return;
free_and_return:
    free(coloredRooms);
    return nextRoom;
}

bool isRoomLeadingToDestination(room *initialRoom, room *destinationRoom, coloredRoom *coloredRooms, size_t roomListSize, crewMember **crewList, size_t crewListSize)
{
    size_t coloredRoomIndex = 0;
    coloredRoom *cRoom = NULL;

    if (modelCountCrewInRoom(initialRoom->id, crewList, crewListSize) >= initialRoom->crewCapacity)
        return false;
    if (initialRoom == destinationRoom) 
        return true;

    while ((cRoom = &(coloredRooms[coloredRoomIndex]))->roomPointer != initialRoom)
        coloredRoomIndex++;

    if (cRoom->isColored)
        return false;
    cRoom->isColored = true;

    for (size_t i = 0; i < initialRoom->adjacentRoomsArraySize; i++)
        if (isRoomLeadingToDestination(initialRoom->adjacentRoomsArray[i], destinationRoom, coloredRooms, roomListSize, crewList, crewListSize))
            return true;

    return false;
}

int modelCrewMemberGoTo(crewMember *crewMemberToMove, size_t roomIdNewDestination, room **roomList, size_t roomListSize)
{
    if (crewMemberToMove->currentRoom->id == roomIdNewDestination)
        goto error_destination_already_reached;

    for (size_t i = 0; i < roomListSize; i++)
    {
        if (roomList[i]->id == roomIdNewDestination)
        {
            crewMemberToMove->destinationRoom = roomList[i];
            return 0;
        }
    }

    displayError("The room does not exist");
    return -1;
error_destination_already_reached:
    displayError("The crew member is already in this room");
    return -2;
}