#include "crew_model.h"

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

int initCrew(int crewSize)
{
    crewMember *crewMember_1 = malloc(sizeof(crewMember));
    if (crewMember_1 == NULL)
    {
        printf("ERROR: Failed to allocate memory for initial crewman");
        return 1;
    }

    strncpy(crewMember_1->name, "James", sizeof(crewMember_1->name));
    crewMember_1->job = ENGINEER;
    addCrewMember(crewMember_1);

    return 0;
}