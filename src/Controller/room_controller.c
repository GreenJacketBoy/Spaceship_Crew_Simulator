#include "room_controller.h"

int handleRoomCommand(char* cmd, size_t maxCmdLength)
{
    if (strncmp(cmd, "room ls\n", maxCmdLength) == 0)
    {
        listRooms();
        return 0;
    }
    
    printf("'%s' is not a valid room command.\n", cmd);
    return -1;
}