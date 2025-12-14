#ifndef SUPER_CONTROLLER_H
#define SUPER_CONTROLLER_H

#include <string.h>
#include "crew_controller.h"
#include "room_controller.h"
#include "crew_room_linker_controller.h"
#include "../core.h"

enum controller {
    CREW,
    ROOM,
    CREW_ROOM_LINKER,
};

int promptAndRedirect();
int getControllerToRedirectTo(char *fullCommand, size_t maxCmdLength, enum controller *controllerToRedirectTo);

#endif