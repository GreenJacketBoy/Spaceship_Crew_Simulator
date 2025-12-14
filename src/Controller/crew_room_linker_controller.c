#include "crew_room_linker_controller.h"

int handleCrewRoomLinkerCommand(char* cmd, size_t maxCmdLength)
{
    if (strncmp(cmd, "linker ls\n", maxCmdLength) == 0)
    {
        displayCrewRoomLinker(crewRoomLinker, crewRoomLinkerSize);
        return 0;
    }
    
    printf("'%s' is not a valid crew room linker command.\n", cmd);
    return -1;
}
