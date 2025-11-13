#include "core.h"

int exitProgramIfStdinClosed()
{
    if (feof(stdin) != 0)
        fuckShitExitNow(ERROR_CODE_STDIN_CLOSED);

    return 0;
}

void fuckShitExitNow(int errorCode)
{
    switch (errorCode)
    {
        case -1:
            printf(RED BOLD "\nERROR : The stdin stream has been closed by the user (likely with Ctrl+D)" CRESET "\n");
            break;
        default:
            printf(RED BOLD "\nERROR : An unspecified error has caused the termination of the program" CRESET "\n");
            break;
    }

    exit(EXIT_FAILURE);
}