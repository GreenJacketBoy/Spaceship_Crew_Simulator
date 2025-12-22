#ifndef CREW_CONTROLLER_H
#define CREW_CONTROLLER_H

#include <stdio.h>
#include "../Model/crew_model.h"
#include "../View/crew_view.h"
#include "../core.h"


int handleCrewCommand(char* cmd, size_t maxCmdLength, room **roomList, size_t roomListSize);
int crewAdd();
int crewRm();
int crewEdit();
int crewMv(room **roomList, size_t roomListSize);
int crewGoto(room **roomList, size_t roomListSize);
int crewShow();

#endif
