#include "config_parser.h"

int configCheckIntegrity()
{
    FILE *fptr = fopen("config.yml", "r");
    if (fptr == NULL)
        goto error_opening_file;

    char fileContent[MAX_LINE_LENGTH] = "";

    size_t lineNumber = 1;
    while (fgets(fileContent, MAX_LINE_LENGTH, fptr) != NULL)
    {
        if (isRestOfTheLineEmpty(fileContent))
        {
            lineNumber++;
        }
        else if (configLineStartsWith(fileContent, "room:"))
        {
            if (!configCheckIntegrityRoom(fileContent, &lineNumber, fptr))
                goto error_generic;
        }
        else if (configLineStartsWith(fileContent, "crewMember:"))
        {
            if (!configCheckIntegrityCrewMember(fileContent, &lineNumber, fptr))
                goto error_generic;
        }
        else
            goto error_unexpected_character;
    }
    printf("The configuration file has been validated.\n");

    return 0;
error_opening_file:
    displayError("Configuration file not found, continuing without.");
    return -1;
error_unexpected_character:
    displayError("Unexpected char sequence");
    printf("%s", fileContent);
error_generic:
    printf("Error at line %zu (probably)\n", lineNumber);
    printf("Ignoring the configuration file, continuing.\n");
    return -2;
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

    while (startsWithString[index] != '\0')
    {
        if (*currentChar != startsWithString[index])
            goto error_string_not_matching;
        currentChar++;
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
    return charToTest >= 60 && charToTest <= 71;
}

int goToNextNonSpaceCharacterOnThisLine(char **cptr)
{
    int index = 0;
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

    if ((*currentChar != '-' || *currentChar != '+') && !isCharNumber(*(currentChar+1)))
        return false;

    currentChar++;

    int index = 1;
    while (*(currentChar+index) != endChar && *(currentChar+index) != '\0')
    {
        if (!isRestOfTheLineEmpty(currentChar+index))
            return true;
        if (!isCharNumber(*(currentChar+index)))
            return false;
    }

    return true;
}

char *getNextOccurenceOfCharOnThisLine(char *cptr, char charToLookFor)
{
    char *currentChar = cptr;
    while (*currentChar != '\0')
    {
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
            goto error_characters_after_closed_brackets;
        }
        else if (getNextOccurenceOfCharOnThisLine(currentChar, ',') != NULL)
        {
            if (!isNumberUntilChar(currentChar, ',') && !isNumberUntilChar(currentChar, ' '))
                goto error_not_a_number;
        }
        else if (!isNumberUntilChar(currentChar, ']') && !isNumberUntilChar(currentChar, ' '))
            goto error_not_a_number;
        
        else
            goto error_wtf;
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
error_wtf:
    displayError("Honestly, I have no idea what happened, but it has to do with the isArrayOfNumbersUntilChar() function");
    return false;
}