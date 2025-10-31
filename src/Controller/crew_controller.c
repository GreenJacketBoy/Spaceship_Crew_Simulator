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
 
    if (strncmp(cmd, "crew rm\n", maxCmdLength) == 0)
    {
        crewRm();
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

int crewRm()
{
    if (crewListSize == 0)
        goto error_empty_crew_list;

    size_t crewMemberId;
    viewRemoveCrewMember(&crewMemberId);

    int errorCode = destroyCrewMember(crewMemberId); 
    return handleRemoveCrewMemberErrors(errorCode);

error_empty_crew_list:
    return handleRemoveCrewMemberErrors(-2);
}