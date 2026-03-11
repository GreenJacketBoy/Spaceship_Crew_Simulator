#ifndef ROOM_CONTROLLER_H
#define ROOM_CONTROLLER_H

#include <stdio.h>
#include "../Model/room_model.h"
#include "../Model/global_variables.h"
#include "../View/room_view.h"
#include "../core.h"


int handleRoomCommand(char* cmd, size_t maxCmdLength, crewMember **crewList, size_t crewListSize);
int roomLs();
int roomAdd();
int roomRm();
int roomLink();
int roomUnlink();
int roomEdit();
int roomShow(crewMember **crewList, size_t crewListSize);

#endif
