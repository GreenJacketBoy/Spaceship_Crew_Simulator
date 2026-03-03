#include "config_parser.h"

int configCheckIntegrity()
{
    switch (configCheckIntegrityAllFieldsPresentForAllObjects(CONFIG_FILE_NAME))
    {
        case -1: goto error_opening_file;
        case -2: goto error_generic;
        default: break;
    }

    switch (configCheckIntegrityAttributesAreTheCorrectType(CONFIG_FILE_NAME))
    {
        case -1: goto error_opening_file;
        case -2: goto error_generic;
        default: break;
    }

    switch (configCheckIntegrityNoDuplicateIds(CONFIG_FILE_NAME))
    {
        case -1: goto error_opening_file;
        case -2: goto error_generic;
        default: break;
    }

    printf("The configuration file has been validated.\n");

    return 0;
error_opening_file:
    displayError("Configuration file not found, continuing without.");
    return -1;
error_generic:
    printf("Ignoring the configuration file, continuing.\n");
    return -2;
}

int configCheckIntegrityAllFieldsPresentForAllObjects(char *configFilePath)
{
    FILE *fptr = fopen(configFilePath, "r");
    if (fptr == NULL)
        goto error_opening_file;

    char lineBuffer[MAX_LINE_LENGTH] = "";

    size_t lineNumber = 1;
    while (fgets(lineBuffer, MAX_LINE_LENGTH, fptr) != NULL)
    {
        if (isRestOfTheLineEmpty(lineBuffer))
        {
            lineNumber++;
        }
        else if (configLineStartsWith(lineBuffer, "room:"))
        {
            if (!configCheckIntegrityRoom(lineBuffer, &lineNumber, fptr))
                goto error_generic;
        }
        else if (configLineStartsWith(lineBuffer, "crewMember:"))
        {
            if (!configCheckIntegrityCrewMember(lineBuffer, &lineNumber, fptr))
                goto error_generic;
        }
        else
            goto error_unexpected_character;
    }

    return 0;

error_opening_file:
    return -1;
error_unexpected_character:
    displayError("Unexpected char sequence");
    printf("%s\n", lineBuffer);
error_generic:
    printf("Error at line %zu (probably)\n", lineNumber);
    return -2;
}

int configCheckIntegrityAttributesAreTheCorrectType(char *configFilePath)
{
    FILE *fptr = fopen(configFilePath, "r");
    if (fptr == NULL)
        goto error_opening_file;

    char lineBuffer[MAX_LINE_LENGTH] = "";

    size_t lineNumber = 1;
    int errorCode = 0;
    while (fgets(lineBuffer, MAX_LINE_LENGTH, fptr) != NULL)
    {
        if ((errorCode = assertStartsWithAndIsExpectedType(lineBuffer, INT, "  id:")) != -1)
        {
            if (errorCode == -2)
                goto error_wrong_type_expecting_int;
        }
        else if ((errorCode = assertStartsWithAndIsExpectedType(lineBuffer, STRING, "  name:")) != -1)
        {
            if (errorCode == -2)
                goto error_wrong_type_expecting_string;
        }
        else if ((errorCode = assertStartsWithAndIsExpectedType(lineBuffer, INT, "  job:")) != -1)
        {
            if (errorCode == -2)
                goto error_wrong_type_expecting_int;
        }
        else if ((errorCode = assertStartsWithAndIsExpectedType(lineBuffer, INT, "  type:")) != -1)
        {
            if (errorCode == -2)
                goto error_wrong_type_expecting_int;
        }
        else if ((errorCode = assertStartsWithAndIsExpectedType(lineBuffer, INT, "  size:")) != -1)
        {
            if (errorCode == -2)
                goto error_wrong_type_expecting_int;
        }
        else if ((errorCode = assertStartsWithAndIsExpectedType(lineBuffer, INT, "  maxCrew:")) != -1)
        {
            if (errorCode == -2)
                goto error_wrong_type_expecting_int;
        }
        else if ((errorCode = assertStartsWithAndIsExpectedType(lineBuffer, INT, "  storage:")) != -1)
        {
            if (errorCode == -2)
                goto error_wrong_type_expecting_int;
        }
        else if ((errorCode = assertStartsWithAndIsExpectedType(lineBuffer, INT, "  currentRoom:")) != -1)
        {
            if (errorCode == -2)
                goto error_wrong_type_expecting_int;
        }
        else if ((errorCode = assertStartsWithAndIsExpectedType(lineBuffer, ARRAY_OF_INT, "  adjacentRooms:")) != -1)
        {
            if (errorCode == -2)
                goto error_wrong_type_expecting_array_of_int;
        }

        lineNumber++;
    }

    return 0;

error_wrong_type_expecting_string:
    displayError("Wrong Type, expected a String");
    goto error_generic;
error_wrong_type_expecting_int:
    displayError("Wrong Type, expected an Int");
    goto error_generic;
error_wrong_type_expecting_array_of_int:
    displayError("Wrong Type, expected an Array of Int");
    goto error_generic;
error_generic:
    printf("%s\n", lineBuffer);
    printf("Error at line %zu (probably)\n", lineNumber);
    return -2;
error_opening_file:
    return -1;
}

int configCheckIntegrityNoDuplicateIds(char *configFilePath)
{
    enum currentlyChecking 
    {
        NOTHING,
        ROOM,
        CREWMEMBER,
    };

    enum currentlyChecking checking = NOTHING;
    size_t roomCount = 0, crewCount = 0, roomIndex = 0, crewIndex = 0;

    char lineBuffer[MAX_LINE_LENGTH] = "";
    size_t lineNumber = 1;
    int errorCode = 0;

    if (countRoomsAndCrew(&roomCount, &crewCount, configFilePath) != 0)
        goto error_opening_file;


    size_t *roomIds = malloc(sizeof(size_t) * roomCount);
    size_t *crewIds = malloc(sizeof(size_t) * crewCount);
    // These 2 arrays store the line number (config file) of each id, so that 
    // if the same id is found twice, the error message can tell which lines to change
    size_t *roomIdsLineNumbers = malloc(sizeof(size_t) * roomCount);
    size_t *crewIdsLineNumbers = malloc(sizeof(size_t) * crewCount);

    FILE *fptr = fopen(configFilePath, "r");
    if (fptr == NULL)
        goto error_opening_file;

    while (fgets(lineBuffer, MAX_LINE_LENGTH, fptr) != NULL)
    {
        if (configLineStartsWith(lineBuffer, "room:")) checking = ROOM;
        else if (configLineStartsWith(lineBuffer, "crewMember:")) checking = CREWMEMBER;
        else if (configLineStartsWith(lineBuffer, "  id:"))
        {
            if (checking == ROOM)
            {
                roomIds[roomIndex] = configGetIntAfterString(lineBuffer, "  id:");
                roomIdsLineNumbers[roomIndex] = lineNumber;
                roomIndex++;
            }
            else if (checking == CREWMEMBER)
            {
                crewIds[crewIndex] = configGetIntAfterString(lineBuffer, "  id:");
                crewIdsLineNumbers[crewIndex] = lineNumber;
                crewIndex++;
            }
        }
        lineNumber++;
    }

    if (hasDuplicates(roomIds, roomIdsLineNumbers, roomCount) || hasDuplicates(crewIds, crewIdsLineNumbers, crewCount))
        goto error_generic;

    goto free_and_return;

error_generic:
    errorCode = -2;
    goto free_and_return;
error_opening_file:
    errorCode = -1;
    goto free_and_return;

free_and_return:

    free(roomIds);
    roomIds = NULL;
    free(crewIds);
    crewIds = NULL;
    free(roomIdsLineNumbers);
    roomIdsLineNumbers = NULL;
    free(crewIdsLineNumbers);
    crewIdsLineNumbers = NULL;

    return errorCode;
}

    
int countRoomsAndCrew(size_t *roomCount, size_t *crewCount, char *configFilePath)
{
    FILE *fptr = fopen(configFilePath, "r");
    if (fptr == NULL)
        goto error_opening_file;

    char lineBuffer[MAX_LINE_LENGTH] = "";

    while (fgets(lineBuffer, MAX_LINE_LENGTH, fptr) != NULL)
    {
        if (configLineStartsWith(lineBuffer, "room:"))
            (*roomCount)++;
        else if (configLineStartsWith(lineBuffer, "crewMember:"))
            (*crewCount)++;
    }

    return 0;

error_opening_file:
    return -1;
}

bool hasDuplicates(size_t *arrayToCheck, size_t *lineNumbersArray, size_t size)
{
    size_t firstDuplicateIndex, secondDuplicateIndex;

    for (size_t i = 0; i < size; i++)
    {
        int instancesFound = 0;
        for (size_t j = 0; j < size; j++)
        {
            if (arrayToCheck[i] == arrayToCheck[j])
            {
                instancesFound++;
                if (instancesFound > 1)
                {
                    firstDuplicateIndex = lineNumbersArray[i];
                    secondDuplicateIndex = lineNumbersArray[j];
                    goto duplicate_found;
                }
            }
        }
    }
    return false;
duplicate_found:
    printf("Error : non-distinct ids at line %zu and %zu\n", firstDuplicateIndex, secondDuplicateIndex);
    return true;
}

size_t configGetIntAfterString(char *cptr, char *startsWithString)
{
    int index = 0;
    char *numberCharPtr;

    while (startsWithString[index] != '\0')
    {
        if (cptr[index] != startsWithString[index])
            goto error_wrong_starts_with_string;        
        index++;
    }

    numberCharPtr = cptr + index;

    char *currentChar = numberCharPtr;
    while (*currentChar != '\0')
    {
        if (*currentChar == '#')
            *currentChar = '\0';
        else
            currentChar++;
    }

    char *endptr;
    return strtoul(numberCharPtr, &endptr, 0);

error_wrong_starts_with_string:
    return -1;
}

bool configLineStartsWith(char *cptr, char *stringToMatch)
{
    int index = 0;

    while (stringToMatch[index] != '\0')
    {
        if (*(cptr + index) != stringToMatch[index])
            return false;
        index++;
    }

    return true;
}

bool isRestOfTheLineEmpty(char *cptr)
{
    char *currentChar = cptr;

    if (*currentChar == '\0' || *currentChar == '\n')
        return true;
    while (*currentChar != '\0')
    {
        if (*currentChar == '#')
            return true;
        else if (*currentChar != ' ' && *currentChar != '\0')
            return false;
        currentChar++;
    }

    return true;
}

/** @returns A pointer to the last checked line, or NULL in case of failure */
bool configCheckIntegrityCrewMember(char *lineBuffer, size_t *lineNumber, FILE *fptr)
{
    bool idPresent = false;
    bool namePresent = false;
    bool jobPresent = false;
    bool currentRoomPresent = false;
 
    while (fgets(lineBuffer, MAX_LINE_LENGTH, fptr) != NULL)
    {
        (*lineNumber)++;
        if (configLineStartsWith(lineBuffer, "  id:"))
            idPresent = true;
        else if (configLineStartsWith(lineBuffer, "  name:"))
            namePresent = true;
        else if (configLineStartsWith(lineBuffer, "  job:"))
            jobPresent = true;
        else if (configLineStartsWith(lineBuffer, "  currentRoom:"))
            currentRoomPresent = true;
        else if (!isRestOfTheLineEmpty(lineBuffer))
            goto error_unexpected_or_missing_attribute;

        if (idPresent && namePresent && jobPresent && currentRoomPresent)
            return true;
    }

error_unexpected_or_missing_attribute:
    displayError("Unexpected or missing attribute in crewMember");
    printf("%s\n", lineBuffer);

    if (!idPresent)
        printf("- id is missing.\n");
    if (!namePresent)
        printf("- name is missing.\n");
    if (!jobPresent)
        printf("- job is missing.\n");
    if (!currentRoomPresent)
        printf("- currentRoom is missing.\n");

    return false;
}

/** @returns A pointer to the last checked line, or NULL in case of failure */
bool configCheckIntegrityRoom(char *lineBuffer, size_t *lineNumber, FILE *fptr)
{
    bool idPresent = false;
    bool namePresent = false;
    bool typePresent = false;
    bool sizePresent = false;
    bool maxCrewPresent = false;
    bool storagePresent = false;
    bool adjacentRoomsPresent = false;

    while (fgets(lineBuffer, MAX_LINE_LENGTH, fptr) != NULL)
    {
        (*lineNumber)++;
        if (configLineStartsWith(lineBuffer, "  id:"))
            idPresent = true;
        else if (configLineStartsWith(lineBuffer, "  name:"))
            namePresent = true;
        else if (configLineStartsWith(lineBuffer, "  type:"))
            typePresent = true;
        else if (configLineStartsWith(lineBuffer, "  size:"))
            sizePresent = true;
        else if (configLineStartsWith(lineBuffer, "  maxCrew:"))
            maxCrewPresent = true;
        else if (configLineStartsWith(lineBuffer, "  storage:"))
            storagePresent = true;
        else if (configLineStartsWith(lineBuffer, "  adjacentRooms:"))
            adjacentRoomsPresent = true;
        else if (!isRestOfTheLineEmpty(lineBuffer))
            goto error_unexpected_or_missing_attribute;

        if (idPresent && namePresent && typePresent && sizePresent && maxCrewPresent && storagePresent && adjacentRoomsPresent)
            return true;
    }

error_unexpected_or_missing_attribute:
    displayError("Unexpected or missing attribute for room");
    printf("%s\n", lineBuffer);

    if (!idPresent)
        printf("- id is missing.\n");
    if (!namePresent)
        printf("- name is missing.\n");
    if (!typePresent)
        printf("- type is missing.\n");
    if (!sizePresent)
        printf("- size is missing.\n");
    if (!maxCrewPresent)
        printf("- maxCrew is missing.\n");
    if (!storagePresent)
        printf("- storage is missing.\n");
    if (!adjacentRoomsPresent)
        printf("- adjacentRooms is missing.\n");

    return false;
}

int assertStartsWithAndIsExpectedType(char *cptr, enum configType expectedType, char *startsWithString)
{
    char *currentChar = cptr;

    int index = 0;

    while (*startsWithString != '\0')
    {
        if (*currentChar != *startsWithString)
            goto error_string_not_matching;
        currentChar++;
        startsWithString++;
    }

    if (expectedType == STRING)
        return 0;
    else if (isRestOfTheLineEmpty(currentChar))
        goto error_matching_but_wrong_type;
    else if (expectedType == INT)
    {
        if (isNumberUntilChar(currentChar, ' ') || isNumberUntilChar(currentChar, '\n'))   
            return 0;
        else
            goto error_matching_but_wrong_type;
    }
    else if (expectedType == ARRAY_OF_INT)
    {
        if (isArrayOfNumbersUntilChar(currentChar, ' ') || isArrayOfNumbersUntilChar(currentChar, '\n'))
            return 0;
        else
            goto error_matching_but_wrong_type;
    }

error_string_not_matching:
    return -1;
error_matching_but_wrong_type:
    return -2;
}

bool isCharNumber(char charToTest)
{
    return charToTest >= 48 && charToTest <= 57;
}

int goToNextNonSpaceCharacterOnThisLine(char **cptr)
{
    while (**cptr != '\0')
    {
        if (**cptr != ' ')
            return 0;
        (*cptr)++;
    }
    displayError("Something wrong happened in the isCharNumber() function");
    return -1;
}

bool isNumberUntilChar(char *cptr, char endChar)
{

    char *currentChar = cptr;
    goToNextNonSpaceCharacterOnThisLine(&currentChar);

    if (*currentChar == '-' || *currentChar == '+') // if first char is - or +
    {
        if (!isCharNumber(*(currentChar+1))) // but second char is not a number
            return false;
    }
    else if (!isCharNumber(*currentChar))
        return false;

    currentChar++;

    while (*currentChar != endChar && *currentChar != '\0')
    {
        if (isRestOfTheLineEmpty(currentChar))
            return true;
        if (!isCharNumber(*currentChar))
            return false;
        currentChar++;
    }

    return true;
}

char *getNextOccurenceOfCharOnThisLine(char *cptr, char charToLookFor)
{
    char *currentChar = cptr;
    while (*currentChar != '\0')
    {
        if (*currentChar == '#') // ignore comments
            return NULL;
        if (*currentChar == charToLookFor)
            return currentChar;
        currentChar++;
    }

    return NULL;
}

bool isArrayOfNumbersUntilChar(char *cptr, char endChar)
{
    char *currentChar = cptr;
    goToNextNonSpaceCharacterOnThisLine(&currentChar);

    if (*currentChar != '[')
        return false;

    currentChar++;

    if (getNextOccurenceOfCharOnThisLine(currentChar, ']') == NULL)
        goto error_bracket_not_closed;

    while (true)
    {
        goToNextNonSpaceCharacterOnThisLine(&currentChar);
        if (*currentChar == ']')
        {
            currentChar++;
            if (isRestOfTheLineEmpty(currentChar))
                return true;
            else
                goto error_characters_after_closed_brackets;
        }
        else if (getNextOccurenceOfCharOnThisLine(currentChar, ',') != NULL)
        {
            if (!isNumberUntilChar(currentChar, ',')  )
            {
                if (isNumberUntilChar(currentChar, ' '))
                {
                    currentChar = getNextOccurenceOfCharOnThisLine(currentChar, ',');

                    // printf("check '%c'\n", *currentChar);
                    currentChar++;
                    // printf("check 2 '%c'\n", *currentChar);
    
                    char *testCharPtr = currentChar; // testing if there is something after the last coma
                    goToNextNonSpaceCharacterOnThisLine(&testCharPtr);
                    // printf("check 3 '%c'\n", *testCharPtr);
                    if (*testCharPtr == ']')
                        goto error_expected_number_after_coma;
                    // else if (*testCharPtr != ',')
                    //     goto error_numbers_separated_by_spaces;
                }
                else
                    goto error_not_a_number;
            }
            else
            {
                currentChar = getNextOccurenceOfCharOnThisLine(currentChar, ',');
                currentChar++;
            }
        }

        else if (isNumberUntilChar(currentChar, ']'))
            return true;
        else if (isNumberUntilChar(currentChar, ' '))
        {
            char *testCharPtr = getNextOccurenceOfCharOnThisLine(currentChar, ' '); // testing if there is something after the last coma
            goToNextNonSpaceCharacterOnThisLine(&testCharPtr);
            if (*testCharPtr != ']')
                goto error_numbers_separated_by_spaces;
            else
                return true;
        }
        else
            goto error_not_a_number;
    }

    return true;
error_bracket_not_closed:
    displayError("A bracket was not closed on the same line");
    return false;
error_characters_after_closed_brackets:
    displayError("There are unexpected characters after the closing bracket");
    return false;
error_not_a_number:
    displayError("One of the elements in the list is not a number");
    return false;
error_expected_number_after_coma:
    displayError("A number was expected after the last coma but nothing was found");
    return false;
error_numbers_separated_by_spaces:
    displayError("Numbers should should be separated by a coma, not by a space");
    return false;
}