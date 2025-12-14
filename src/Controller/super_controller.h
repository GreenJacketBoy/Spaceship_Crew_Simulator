#ifndef SUPER_CONTROLLER_H
#define SUPER_CONTROLLER_H

#include <string.h>
#include "crew_controller.h"
#include "room_controller.h"
#include "../core.h"

enum controller {
    CREW,
    ROOM,
};

int promptAndRedirect();
int getControllerToRedirectTo(char *fullCommand, size_t maxCmdLength, enum controller *controllerToRedirectTo);

#endif