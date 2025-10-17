#include "crew_controller.h"

int handleCrewCommand(char* cmd, size_t maxCmdLength)
{
    if (strncmp(cmd, "crew ls\n", maxCmdLength) == 0)
    {
        listCrew();
        return 0;
    }
    
    printf("'%s' is not a valid crew command.\n", cmd);
    return -1;
}