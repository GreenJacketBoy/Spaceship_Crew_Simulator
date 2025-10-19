#ifndef SUPER_CONTROLLER_H
#define SUPER_CONTROLLER_H

#include <string.h>
#include "../View/super_view.h"
#include "crew_controller.h"
#include "room_controller.h"

enum controller {
    CREW,
    ROOM,
};

int promptAndRedirect();
int getControllerToRedirectTo(char *fullCommand, size_t maxCmdLength, enum controller *controllerToRedirectTo);

#endif