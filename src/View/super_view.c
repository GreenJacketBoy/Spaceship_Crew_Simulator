#include "super_view.h"

int promptForCommand(char *cmdPlusArgs, size_t maxLength) {
    printf("\nEnter a command :\n");
    printf(BOLD);
    if (fgets(cmdPlusArgs, maxLength, stdin) == NULL)
    {
        exitProgramIfStdinClosed();
        goto error_reading_input;
    }
    printf(CRESET);

    return 0;

error_reading_input:
    printf(CRESET);
    return -1;
}

size_t promptForSize_T(char *promptMessage)
{
    if (promptMessage != NULL)
        printf("%s\n", promptMessage);

    char userInput[256] = "";
    printf(BLU BOLD);
    if (fgets(userInput, 256, stdin) == NULL)
    {
        exitProgramIfStdinClosed();
        goto error_reading_input;
    }
    printf(CRESET);
    
    char *endptr;
    size_t number = strtoul(userInput, &endptr, 0); 
    
    // I know I'm not doing a ton of error handling here but it's
    // complicated + it doesn't cause too much problems so whatever

    return number; 

error_reading_input:
    printf(CRESET);
    return 0;
}

int promptForString(char *strBuffer, size_t strBufferLength)
{
    printf(GRN BOLD);
    if (fgets(strBuffer, strBufferLength, stdin) == NULL)
    {
        exitProgramIfStdinClosed();
        goto error_reading_input;
    }
    printf(CRESET);

    char *nameEndLineChar = strchr(strBuffer, '\n');
    if (nameEndLineChar != NULL)
        *nameEndLineChar = '\0';

    return 0;

error_reading_input:
    printf(CRESET);
    return -1;
}