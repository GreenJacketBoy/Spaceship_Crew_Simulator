#ifndef CORE_H
#define CORE_H

#define ERROR_CODE_STDIN_CLOSED -1

#include <stdio.h>
#include "View/colors.h"
#include <stdlib.h>

void fuckShitExitNow(int errorCode);
int exitProgramIfStdinClosed();

#endif