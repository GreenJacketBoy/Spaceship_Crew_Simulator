#ifndef SUPER_VIEW_H
#define SUPER_VIEW_H

#include <stdio.h>
#include <string.h>
#include "colors.h"
#include "stdlib.h"
#include "../core.h"

int promptForCommand(char* cmdPlusArgs, size_t maxLength);
size_t promptForSize_T(char *promptMessage);
int displayError(char *errorMessage);
int promptForString(char *strBuffer, size_t strBufferLength);

#endif