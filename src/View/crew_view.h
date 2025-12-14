#ifndef CREW_VIEW_H
#define CREW_VIEW_H


#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../Model/type_definitions.h"
#include "colors.h"
#include "super_view.h"
#include "../core.h"

int listCrew(crewMember **crewList, size_t crewListSize);
int getJobToString(enum job job, char *jobString, int jobStringMaxSize);
int generateCrewMember(enum job *job, char *name, size_t nameLength);
int printCrewMemberWithPrefix(char *prefix, crewMember *crewMemberToDisplay);
int viewRemoveCrewMember(size_t *crewMemberId);
int handleRemoveCrewMemberErrors(int errorCode);
int viewEditCrewMember(crewMember *crewMemberToEdit, enum job *job, char *name, size_t nameLength);
enum job promptForJob(char *promptMessage);

#endif