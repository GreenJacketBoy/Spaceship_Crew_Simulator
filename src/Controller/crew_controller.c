#include "crew_controller.h"

int handleCrewCommand(char* cmd, size_t maxCmdLength, room **roomList, size_t roomListSize)
{
    if (strncmp(cmd, "crew ls\n", maxCmdLength) == 0)
    {
        listCrew(crewList, crewListSize);
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

    if (strncmp(cmd, "crew edit\n", maxCmdLength) == 0)
    {
        crewEdit();
        return 0;
    }

    if (strncmp(cmd, "crew mv\n", maxCmdLength) == 0)
    {
        crewMv(roomList, roomListSize);
        return 0;
    }

    if (strncmp(cmd, "crew goto\n", maxCmdLength) == 0)
    {
        crewGoto(roomList, roomListSize);
        return 0;
    }
    
    printf("'%s' is not a valid crew command.\n", cmd);
    return -1;
}

int crewAdd()
{
    enum job job;
    char name[CREW_MEMBER_NAME_MAX_LENGTH];

    switch (generateCrewMember(&job, name, CREW_MEMBER_NAME_MAX_LENGTH))
    {
        case -1:
            goto error_reading_input;
    }
 
    crewMember *newCrewMember = buildCrewMember(job, name, CREW_MEMBER_NAME_MAX_LENGTH);
    if (newCrewMember == NULL)
        goto error_malloc_faillure_crew_member;

    if (addCrewMember(newCrewMember) != 0)
        goto error_malloc_faillure_crew_list;

    return 0;

error_malloc_faillure_crew_member:
    displayError("Failed to allocate enough memory for a new crewman");
    return -1;
error_malloc_faillure_crew_list:
    displayError("Failed to allocate enough memory to update the crew list");
    free(newCrewMember);
    return -1;
error_reading_input:
    displayError("There's been an error when parsing the last input");
    return -2;
}

int crewRm()
{
    if (crewListSize == 0)
        goto error_empty_crew_list;

    size_t crewMemberId;
    viewRemoveCrewMember(&crewMemberId);

    int errorCode = destroyCrewMember(crewMemberId); 

    switch (errorCode)
    {
        case 0:
            return 0;
            break;
        case -1:
            goto error_malloc_faillure;           
            break;
        case -2:
            goto error_empty_crew_list;           
            break;
        case -404:
            goto error_crew_member_not_found;
            break;
    }

error_malloc_faillure:
    displayError("Could not allocate enough memory to update the crew member list");
    return -1;
error_empty_crew_list:
    displayError("There are no crew members to remove");
    return -2;
error_crew_member_not_found:
    displayError("The crew member could not be found");
    return -404;
}

int crewEdit()
{
    size_t crewMemberIdToEdit = promptForSize_T("Id of the crew member to edit :");
    crewMember *crewMemberToEdit = getCrewMemberFromArray(crewMemberIdToEdit, crewList, crewListSize);

    if (crewMemberToEdit == NULL)
        goto error_crew_member_not_found;

    enum job newJob = 0;
    char newName[CREW_MEMBER_NAME_MAX_LENGTH] = "";

    if (viewEditCrewMember(crewMemberToEdit, &newJob, newName, CREW_MEMBER_NAME_MAX_LENGTH))
        goto error_reading_input;
    modelEditCrewMember(crewMemberToEdit, newJob, newName, CREW_MEMBER_NAME_MAX_LENGTH);

    return 0;

error_crew_member_not_found:
    displayError("There are no crew members with this Id");
    return -1;
error_reading_input:
    displayError("There's been an error when parsing the last input");
    return -2;
}

int crewMv(room **roomList, size_t roomListSize)
{
    size_t crewMemberIdToMove = promptForSize_T("Id of the crew member to move :");
    crewMember *crewMemberToMove = getCrewMemberFromArray(crewMemberIdToMove, crewList, crewListSize);

    if (crewMemberToMove == NULL)
        goto error_crew_member_not_found;

    size_t roomIdToMoveTo = promptForSize_T("Id of the room to move to :");
    modelMoveCrewMember(crewMemberToMove, roomIdToMoveTo, roomList, roomListSize);

    return 0;

error_crew_member_not_found:
    displayError("There are no crew members with this Id");
    return -1;
}

int crewGoto(room **roomList, size_t roomListSize)
{
    size_t crewMemberIdToMove = promptForSize_T("Id of the crew member whose destination you want to change :");
    crewMember *crewMemberToMove = getCrewMemberFromArray(crewMemberIdToMove, crewList, crewListSize);

    if (crewMemberToMove == NULL)
        goto error_crew_member_not_found;

    size_t roomIdNewDestination = promptForSize_T("Id of the room set to be the new destination :");
    modelCrewMemberGoTo(crewMemberToMove, roomIdNewDestination, roomList, roomListSize);

    return 0;

error_crew_member_not_found:
    displayError("There are no crew members with this Id");
    return -1;
}