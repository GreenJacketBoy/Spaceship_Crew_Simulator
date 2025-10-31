#include "super_view.h"

int promptForCommand(char *cmdPlusArgs, size_t maxLength) {
    printf("\nEnter a command :\n");
    printf(BOLD);
    fgets(cmdPlusArgs, maxLength, stdin);
    printf(CRESET);

    return 0;
}

size_t promptForSize_T(char *promptMessage)
{
    if (promptMessage != NULL)
        printf("%s\n", promptMessage);

    char userInput[256] = "";
    printf(BLU BOLD);
    fgets(userInput, 256, stdin);
    printf(CRESET);
    
    char *endptr;
    size_t number = strtoul(userInput, &endptr, 0); 
    
    // I know I'm not doing any error handling here but it's
    // complicated + it doesn't cause too much problems so whatever

    return number; 
}

int displayError(char *errorMessage)
{
    printf(RED BOLD "ERROR : %s" CRESET "\n", errorMessage);
    return 0;
}