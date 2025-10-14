#ifndef CREW_VIEW_H
#define CREW_VIEW_H

#include <stdio.h>
#include <string.h>
#include "../Model/crew_model.h"

int listCrew();
int getJobToString(enum job job, char *jobString, int *jobStringMaxSize);

#endif