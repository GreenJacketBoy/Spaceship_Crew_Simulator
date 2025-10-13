#include <stdio.h>
#include <string.h>
#include <stdlib.h>

enum job {
    ENGINEER,
};

typedef struct
{
    char name[64];
    enum job job;
} crewMember;

crewMember** crewList;
size_t crewListSize = 0;

int initCrew(int crewSize);
int handleCmdInput(char* cmd);
int addCrewMember(crewMember *newCrewMember);
int listCrew();
int getJobToString(enum job job, char *jobString, int *jobStringMaxSize);

int main() 
{
    initCrew(4);
    
    while (1 == 1)
    {
        char cmd[64];
        printf("Enter a command :\n");
        fgets(cmd, sizeof(cmd), stdin); 

        printf("You've entered : %s\n", cmd);

        char* cmdPtr = cmd;
        
        handleCmdInput(cmdPtr);
    }
}


int handleCmdInput(char* cmd)
{
    if (strcmp(cmd, "crew ls\n") == 0)
    {
        listCrew();
        return 0;
    }
    
    return -1;
}

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

int listCrew() {
    printf("Crew :\n");

    if (crewListSize == 0)
    {
        printf("The crew is empty.\n");
        return 0;
    }
    
    
    for (size_t i = 0; i < crewListSize; i++)
    {
        printf("Name: %s\n", crewList[i]->name);

        int jobStringMaxSize = 64;
        char jobString[jobStringMaxSize];
        getJobToString(crewList[i]->job, jobString, &jobStringMaxSize); // jobToString gets turned into a char pointer when passed

        printf("Job: %s\n", jobString);
    }
    
    return 0;
}

int getJobToString(enum job job, char *jobString, int *jobStringMaxSize) {
    switch (job)
    {
        case ENGINEER:
            strncpy(jobString, "Engineer", *jobStringMaxSize);
            break;
        
        default:
            strncpy(jobString, "Unregistered Job", *jobStringMaxSize);
            break;
    }

    return 0;
}