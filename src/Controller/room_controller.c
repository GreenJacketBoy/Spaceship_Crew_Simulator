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

    addRoom(newRoom);
}