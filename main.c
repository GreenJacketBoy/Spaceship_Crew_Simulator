#include <stdio.h>
#include <string.h>
#include <stdlib.h>

enum job {
    ENGINEER,
};

typedef struct
{
    char name[30];
    enum job job;
} crewMember;

crewMember** crewList;
size_t crewListSize = 0;

int initCrew(int crewSize);
int handleCmdInput(char* cmd);
int addCrewMember(crewMember *newCrewMember);
int listCrew();

int main() 
{
    initCrew(4);
    
    while (1 == 1)
    {
        char cmd[20];
        printf("Enter a command :\n");
        fgets(cmd, 20, stdin); 

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
    strcpy(crewMember_1->name, "James");
    crewMember_1->job = ENGINEER;
    addCrewMember(crewMember_1);
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
        switch ( crewList[i]->job )
        {
        case ENGINEER:
            printf("Job: Engineer\n");
            break;
        
        default:
            printf("Job: Unregistered Job\n");
            break;
        }
    }
    
    return 0;
}