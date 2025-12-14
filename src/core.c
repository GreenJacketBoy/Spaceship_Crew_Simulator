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
            displayError("The stdin stream has been closed by the user (likely with Ctrl+D)");
            break;
        default:
            displayError("An unspecified error has caused the termination of the program");
            break;
    }

    exit(EXIT_FAILURE);
}

int displayError(char *errorMessage)
{
    printf(RED BOLD "ERROR : %s" CRESET "\n", errorMessage);
    return 0;
}
