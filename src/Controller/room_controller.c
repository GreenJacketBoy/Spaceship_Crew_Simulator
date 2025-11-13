#include "room_controller.h"

int handleRoomCommand(char* cmd, size_t maxCmdLength)
{
    if (strncmp(cmd, "room ls\n", maxCmdLength) == 0)
    {
        listRooms();
        return 0;
    }

    if (strncmp(cmd, "room add\n", maxCmdLength) == 0)
    {
        roomAdd();
        return 0;
    }

    if (strncmp(cmd, "room rm\n", maxCmdLength) == 0)
    {
        roomRm();
        return 0;
    }

    if (strncmp(cmd, "room link\n", maxCmdLength) == 0)
    {
        roomLink();
        return 0;
    }

    if (strncmp(cmd, "room unlink\n", maxCmdLength) == 0)
    {
        roomUnlink();
        return 0;
    }

    if (strncmp(cmd, "room edit\n", maxCmdLength) == 0)
    {
        roomEdit();
        return 0;
    }
    
    printf("'%s' is not a valid room command.\n", cmd);
    return -1;
}

int roomAdd()
{
    
    char name[ROOM_NAME_MAX_LENGTH] = "";
    enum roomType roomType;
    size_t crewCapacity = 0;
    size_t storageCapacity = 0;
    size_t size = 0;

    if (promptRoomAttributes((char *) &name, ROOM_NAME_MAX_LENGTH, &roomType, &crewCapacity, &storageCapacity, &size) == -1)
        goto error_reading_input;

    room *newRoom = createRoom(name, roomType, crewCapacity, storageCapacity, size);
    if (newRoom == NULL)
        goto error_malloc_faillure_room;
    if (addRoom(newRoom) != 0)
        goto error_malloc_faillure_room_list;

    return 0;

error_malloc_faillure_room:
    displayError("Failed to allocate enough memory for the a new room");
    return -1;
error_malloc_faillure_room_list:
    displayError("Failed to allocate enough memory to update the room list");
    free(newRoom);
    return -2;
error_reading_input:
    displayError("There's been an error when parsing the last input");
    return -3;
}

int roomRm()
{
    size_t roomIdToRemove;
    viewRoomRm(&roomIdToRemove);

    int errorCode = destroyRoom(roomIdToRemove);

    switch (errorCode) {
        case 0:
            break;
        case -1:
            displayError("There was a problem when attempting to destroy the room");
    }

    return errorCode;
}

int roomLink()
{
    size_t roomIdToLink_1 = promptForSize_T("Id of the " BOLD "first" CRESET " room to link :");
    room *roomToLink_1 = getRoomInArray(roomList, roomListSize, roomIdToLink_1);
    if (roomToLink_1 == NULL)
        goto error_room_not_found;

    size_t roomIdToLink_2 = promptForSize_T("Id of the " BOLD "second" CRESET " room to link :");
    room *roomToLink_2 = getRoomInArray(roomList, roomListSize, roomIdToLink_2);
    if (roomToLink_2 == NULL)
        goto error_room_not_found;

    if (addAdjacentRoom(roomToLink_1, roomToLink_2) != 0)
        goto error_could_not_link_room;

    return 0;

error_room_not_found:
    displayError("There are no rooms with this Id");
    return -1;
error_could_not_link_room:
    displayError("The 2 rooms were found but something went wrong when attempting to link them");
    return -2;
}

int roomUnlink()
{
    size_t roomIdToUnlink_1 = promptForSize_T("Id of the " BOLD "first" CRESET " room to unlink :");
    room *roomToUnlink_1 = getRoomInArray(roomList, roomListSize, roomIdToUnlink_1);
    if (roomToUnlink_1 == NULL)
        goto error_room_not_found;

    size_t roomIdToUnlink_2 = promptForSize_T("Id of the " BOLD "second" CRESET " room to unlink :");
    room *roomToUnlink_2 = getRoomInArray(roomList, roomListSize, roomIdToUnlink_2);
    if (roomToUnlink_2 == NULL)
        goto error_room_not_found;

    if (isAdjacent(roomToUnlink_1, roomToUnlink_2) == false)
        return 0;

    bool deepRemoval = false;
    if (popRoomFromRoomArray(&roomToUnlink_1->adjacentRoomsArray, &roomToUnlink_1->adjacentRoomsArraySize, roomToUnlink_2->id, deepRemoval) == NULL)
        goto error_could_not_unlink_room;

    if (popRoomFromRoomArray(&roomToUnlink_2->adjacentRoomsArray, &roomToUnlink_2->adjacentRoomsArraySize, roomToUnlink_1->id, deepRemoval) == NULL)
        goto error_could_not_unlink_room;

    return 0;

error_room_not_found:
    displayError("There are no rooms with this Id");
    return -1;
error_could_not_unlink_room:
    displayError("The 2 rooms were found but something went wrong when attempting to unlink them");
    return -2;
}

int roomEdit()
{
    size_t roomIdToEdit = promptForSize_T("Id of the room to edit :");
    room *roomToEdit = getRoomInArray(roomList, roomListSize, roomIdToEdit);

    if (roomToEdit == NULL)
        goto error_room_not_found;

    char newName[ROOM_NAME_MAX_LENGTH] = "";
    enum roomType newType = 0;
    size_t newCrewCapacity = 0;
    size_t newStorageCapacity = 0;
    size_t newSize = 0;

    if (viewRoomEdit(roomToEdit, newName, ROOM_NAME_MAX_LENGTH, &newType, &newCrewCapacity, &newStorageCapacity, &newSize) == -1)
        goto error_reading_input;
    modelEditRoom(roomToEdit, newName, ROOM_NAME_MAX_LENGTH, newType, newCrewCapacity, newStorageCapacity, newSize);

    return 0;

error_room_not_found:
    displayError("There are no rooms with this Id");
    return -1;
error_reading_input:
    displayError("There's been an error when parsing the last input");
    return -2;
}