#ifndef CREW_CONTROLLER_H
#define CREW_CONTROLLER_H

#include <stdio.h>
#include "../Model/crew_model.h"
#include "../View/crew_view.h"
#include "../View/super_view.h"


int handleCrewCommand(char* cmd, size_t maxCmdLength);
int crewAdd();
int crewRm();

#endif
