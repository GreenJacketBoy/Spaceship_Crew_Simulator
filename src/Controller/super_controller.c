#include "super_controller.h"

int promptAndRedirect()
{
    size_t maxCommandLength = 64;
    char commandPlusArgs[maxCommandLength];
    
    promptForCommand(commandPlusArgs, maxCommandLength);

    size_t maxWordLength = 64;
    char commandWord[maxWordLength];

    enum controller controllerToRedirectTo;

    if (getControllerToRedirectTo(commandPlusArgs, maxCommandLength, &controllerToRedirectTo) != 0)
    {
        return -1;
    }

    switch (controllerToRedirectTo)
    {
    case CREW:
        handleCrewCommand(commandPlusArgs, maxCommandLength);
        break;
    case ROOM:
        handleRoomCommand(commandPlusArgs, maxCommandLength);
        break;
    default:
        return -2;
    }

    return 0;
}

int getControllerToRedirectTo(char *fullCommand, size_t maxCmdLength, enum controller *controllerToRedirectTo) 
{
    char commandWord[maxCmdLength];
    strncpy(commandWord, fullCommand, maxCmdLength);
    char *firstSpaceChar = strchr(commandWord, ' ');

    if (firstSpaceChar == NULL)
    {
        firstSpaceChar = strchr(commandWord, '\n');

        if (firstSpaceChar == NULL)
        {
            printf("There's been a problem in the parsing of the command word.\n");
            return -1;
        }
        
    }

    *firstSpaceChar = 0;

    if (strncmp(commandWord, "crew", maxCmdLength) == 0)
        goto crew;
    if (strncmp(commandWord, "room", maxCmdLength) == 0)
        goto room;
    else
        goto error_invalid_word;

    crew:
        *controllerToRedirectTo = CREW;
        return 0;
    room:
        *controllerToRedirectTo = ROOM;
        return 0;
    error_invalid_word:
        printf("'%s' is not a valid command word.\n", commandWord);
        return -2;

}