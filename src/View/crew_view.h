#ifndef CREW_VIEW_H
#define CREW_VIEW_H

#define JOB_STRING_MAX_LENGTH 64

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../Model/crew_model.h"
#include "colors.h"

int listCrew();
int getJobToString(enum job job, char *jobString, int jobStringMaxSize);
int generateCrewMember(enum job *job, char *name, size_t nameLength);
int printCrewMemberWithPrefix(char *prefix, crewMember *crewMemberToDisplay);

#endif