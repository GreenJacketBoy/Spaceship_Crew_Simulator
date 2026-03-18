#ifndef CREW_CONTROLLER_H
#define CREW_CONTROLLER_H

#include <stdio.h>
#include "../Model/crew_model.h"
#include "../Model/global_variables.h"
#include "../View/crew_view.h"
#include "../core.h"


int handleCrewCommand(char* cmd, size_t maxCmdLength);
int crewAdd();
int crewRm();
int crewEdit();
int crewMv(room **roomList, size_t roomListSize, crewMember **crewList, size_t crewListSize);
int crewGoto(room **roomList, size_t roomListSize);
int crewShow();

#endif
