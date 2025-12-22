#ifndef ROOM_CONTROLLER_H
#define ROOM_CONTROLLER_H

#include <stdio.h>
#include "../Model/room_model.h"
#include "../View/room_view.h"
#include "../core.h"


int handleRoomCommand(char* cmd, size_t maxCmdLength, crewRoomLink **crewRoomLinker, size_t crewRoomLinkerSize);
int roomLs();
int roomAdd();
int roomRm();
int roomLink();
int roomUnlink();
int roomEdit();
int roomShow(crewRoomLink **crewRoomLinker, size_t crewRoomLinkerSize);

#endif
