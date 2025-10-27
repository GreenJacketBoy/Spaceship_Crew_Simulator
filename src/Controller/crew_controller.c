#include "crew_controller.h"

int handleCrewCommand(char* cmd, size_t maxCmdLength)
{
    if (strncmp(cmd, "crew ls\n", maxCmdLength) == 0)
    {
        listCrew();
        return 0;
    }
    
    if (strncmp(cmd, "crew add\n", maxCmdLength) == 0)
    {
        crewAdd();
        return 0;
    }
    
    printf("'%s' is not a valid crew command.\n", cmd);
    return -1;
}

int crewAdd()
{
    enum job job;
    char name[CREW_MEMBER_NAME_MAX_LENGTH];

    generateCrewMember(&job, name, CREW_MEMBER_NAME_MAX_LENGTH);
    
    crewMember *newCrewMember = buildCrewMember(job, name, CREW_MEMBER_NAME_MAX_LENGTH);
    if (newCrewMember == NULL)
        return -1;

    if (addCrewMember(newCrewMember) != 0)
        return -2;

    return 0;
}