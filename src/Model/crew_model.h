#ifndef CREW_MODEL_H
#define CREW_MODEL_H

#define CREW_MEMBER_NAME_MAX_LENGTH 64
#define AMOUNT_OF_DIFFERENT_JOBS 1

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

enum job { // Try not to forget to update AMOUNT_OF_DIFFERENT_JOBS 
    ENGINEER,
};

typedef struct
{
    size_t id;
    char name[CREW_MEMBER_NAME_MAX_LENGTH];
    enum job job;
} crewMember;

extern size_t currentBiggestCrewMemberId;
extern crewMember** crewList;
extern size_t crewListSize;

int initCrew(int crewSize);
int addCrewMember(crewMember *newCrewMember);
crewMember *buildCrewMember(enum job job, char *name, size_t nameLength);
int destroyCrewMember(size_t crewMemberId);

#endif