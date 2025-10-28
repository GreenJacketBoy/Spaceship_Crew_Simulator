#include "crew_model.h"

size_t currentBiggestCrewMemberId = 0;
crewMember** crewList = NULL;
size_t crewListSize = 0;


int addCrewMember(crewMember *newCrewMember)
{
    crewMember **newCrewList = malloc(sizeof(crewMember*) * (crewListSize + 1));
    if (newCrewList == NULL) 
    {
        printf("ERROR: Failed to allocate enough memory to add a crewman to the list");
        return 1;
    }
    for (size_t i = 0; i < crewListSize; i++)
    {
        newCrewList[i] = crewList[i];
    }

    newCrewList[crewListSize] = newCrewMember;
    crewListSize++;

    free(crewList);

    crewList = newCrewList;
    return 0;
}

crewMember *buildCrewMember(enum job job, char *name, size_t nameLength)
{
    crewMember *newCrewMember = malloc(sizeof(crewMember));
    if (newCrewMember == NULL)
    {
        printf("ERROR: Failed to allocate memory for a crewman");
        return NULL;
    }

    currentBiggestCrewMemberId++;

    newCrewMember->id = currentBiggestCrewMemberId;
    strncpy(newCrewMember->name, name, CREW_MEMBER_NAME_MAX_LENGTH);
    newCrewMember->job = job;

    return newCrewMember;
}

int initCrew(int crewSize)
{
    crewMember *newCrewMember = buildCrewMember(ENGINEER, "James", CREW_MEMBER_NAME_MAX_LENGTH);
    if (newCrewMember == NULL)
        return -1;
    
    if (addCrewMember(newCrewMember) != 0)
        return -2;

    return 0;
}