#ifndef CREW_ROOM_LINKER_CONTROLLER_H
#define CREW_ROOM_LINKER_CONTROLLER_H

#include <stdio.h>
#include <string.h>
#include "../Model/crew_room_linker_model.h"
#include "../View/crew_room_linker_view.h"
#include "../core.h"

int handleCrewRoomLinkerCommand(char* cmd, size_t maxCmdLength);

#endif
