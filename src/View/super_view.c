#include "super_view.h"

int promptForCommand(char *cmdPlusArgs, size_t maxLength) {
    printf("\nEnter a command :\n");
    printf(BOLD);
    fgets(cmdPlusArgs, maxLength, stdin);
    printf(CRESET);

    return 0;
}