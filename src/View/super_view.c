#include "super_view.h"

int promptForCommand(char *cmdPlusArgs, size_t maxLength) {
    printf("\nEnter a command :\n");
    fgets(cmdPlusArgs, maxLength, stdin);

    return 0;
}