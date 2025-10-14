#ifndef CREW_MODEL_H
#define CREW_MODEL_H

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

extern crewMember** crewList;
extern size_t crewListSize;

int initCrew(int crewSize);
int addCrewMember(crewMember *newCrewMember);

#endif