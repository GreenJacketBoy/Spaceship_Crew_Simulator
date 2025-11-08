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

    promptRoomAttributes((char *) &name, ROOM_NAME_MAX_LENGTH, &roomType, &crewCapacity, &storageCapacity, &size);

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