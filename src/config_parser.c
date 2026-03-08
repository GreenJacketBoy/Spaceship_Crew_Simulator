#include "config_parser.h"

int configCheckIntegrityAndFillParams(crewMember ***crewMembers, size_t *crewCount, room ***rooms, size_t *roomCount)
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

    switch (configCheckIntegrityNoDuplicateIds(crewMembers, crewCount, rooms, roomCount, CONFIG_FILE_NAME))
    {
        case -1: goto error_opening_file;
        case -2: goto error_generic;
        default: break;
    }

    switch (configCheckIntegrityReferencedIdsExist(*crewMembers, *crewCount, *rooms, *roomCount, CONFIG_FILE_NAME))
    {
        case -1: goto error_opening_file;
        case -2: goto error_generic;
        default: break;
    }

    switch (configSetAllRemainingFields(*crewMembers, *crewCount, *rooms, *roomCount, CONFIG_FILE_NAME))
    {
        case -1: goto error_opening_file;
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

int configCheckIntegrityNoDuplicateIds(crewMember ***crewMembers, size_t *crewCount, room ***rooms, size_t *roomCount, char *configFilePath)
{
    enum currentlyChecking checking = CHECKING_NOTHING;
    size_t roomIndex = 0, crewIndex = 0;

    char lineBuffer[MAX_LINE_LENGTH] = "";
    size_t lineNumber = 1;
    int errorCode = 0;

    if (countRoomsAndCrew(roomCount, crewCount, configFilePath) != 0)
        goto error_opening_file;

    *rooms = malloc(sizeof(room) * *roomCount);
    *crewMembers = malloc(sizeof(crewMember) * *crewCount);
    // These 2 arrays store the line number (config file) of each id, so that 
    // if the same id is found twice, the error message can tell which lines to change
    size_t *roomIdsLineNumbers = malloc(sizeof(size_t) * *roomCount);
    size_t *crewIdsLineNumbers = malloc(sizeof(size_t) * *crewCount);


    FILE *fptr = fopen(configFilePath, "r");
    if (fptr == NULL)
        goto error_opening_file;

    while (fgets(lineBuffer, MAX_LINE_LENGTH, fptr) != NULL)
    {
        if (configLineStartsWith(lineBuffer, "room:")) checking = CHECKING_ROOM;
        else if (configLineStartsWith(lineBuffer, "crewMember:")) checking = CHECKING_CREWMEMBER;
        else if (configLineStartsWith(lineBuffer, "  id:"))
        {
            if (checking == CHECKING_ROOM)
            {
                (*rooms)[roomIndex] = malloc(sizeof(room));
                (*rooms)[roomIndex]->id = configGetIntAfterString(lineBuffer, "  id:");
                (*rooms)[roomIndex]->adjacentRoomsArray = NULL; // in case of a later free
                roomIdsLineNumbers[roomIndex] = lineNumber;
                roomIndex++;
            }
            else if (checking == CHECKING_CREWMEMBER)
            {
                (*crewMembers)[crewIndex] = malloc(sizeof(crewMember));
                (*crewMembers)[crewIndex]->id = configGetIntAfterString(lineBuffer, "  id:");
                crewIdsLineNumbers[crewIndex] = lineNumber;
                crewIndex++;
            }
        }
        lineNumber++;
    }

    if (roomHasDuplicatesIds(*rooms, roomIdsLineNumbers, *roomCount) || crewMemberHasDuplicatesIds(*crewMembers, crewIdsLineNumbers, *crewCount))
        goto error_generic;

    goto free_line_numbers_and_return;

error_opening_file:
    return -1;
error_generic:
    errorCode = -2;
    freeAll(*crewMembers, *crewCount, *rooms, *roomCount);

free_line_numbers_and_return:
    free(roomIdsLineNumbers);
    roomIdsLineNumbers = NULL;
    free(crewIdsLineNumbers);
    crewIdsLineNumbers = NULL;

    return errorCode;
}

int configCheckIntegrityReferencedIdsExist(crewMember **crewMembers, size_t crewCount, room **rooms, size_t roomCount, char *configFilePath)
{
    enum currentlyChecking checking = CHECKING_NOTHING;


    char lineBuffer[MAX_LINE_LENGTH] = "";
    size_t id = 0;
    size_t roomIndex = -1;
    size_t lineNumber = 1;
    int errorCode = 0;

    FILE *fptr = fopen(configFilePath, "r");
    if (fptr == NULL)
        goto error_opening_file;

    while (fgets(lineBuffer, MAX_LINE_LENGTH, fptr) != NULL)
    {
        if (configLineStartsWith(lineBuffer, "room:")) {checking = CHECKING_ROOM; roomIndex++;}
        else if (configLineStartsWith(lineBuffer, "crewMember:")) checking = CHECKING_CREWMEMBER;
        else if (checking == CHECKING_CREWMEMBER)
        {
            if (configLineStartsWith(lineBuffer, "  currentRoom:") && !isRoomInArray(rooms, roomCount, id = configGetIntAfterString(lineBuffer, "  currentRoom:")))
            {
                printf("Error : non-existing room id (%zu) used at line %zu\n%s\n", id, lineNumber, lineBuffer);
                goto error_generic;
            }
            else if (configLineStartsWith(lineBuffer, "  job:") && (id = configGetIntAfterString(lineBuffer, "  job:")) > AMOUNT_OF_DIFFERENT_JOBS)
            {
                printf("Error : non-existing job index (%zu) used at line %zu\nJob index cannot exceed the amount of different jobs (%d)\n%s\n", id, lineNumber, AMOUNT_OF_DIFFERENT_JOBS, lineBuffer);
                goto error_generic;
            }
        }
        else if (checking == CHECKING_ROOM)
        {
            if (configLineStartsWith(lineBuffer, "  type:") && (id = configGetIntAfterString(lineBuffer, "  type:")) > AMOUNT_OF_DIFFERENT_ROOM_TYPES)
            {
                printf("Error : non-existing room type index (%zu) used at line %zu\nRoom type index cannot exceed the amount of different room types (%d)\n%s\n", id, lineNumber, AMOUNT_OF_DIFFERENT_JOBS, lineBuffer);
                goto error_generic;
            }
            else if (configLineStartsWith(lineBuffer, "  adjacentRooms:"))
            {
                char *charPtr = getNextOccurenceOfCharOnThisLine(lineBuffer, '[') + 1;
                if (initAndCheckAdjacentRooms(rooms, roomCount, rooms[roomIndex], charPtr, lineNumber) != 0)
                    goto error_generic;
            }
        }
        lineNumber++;
    }

    return errorCode;

error_generic:
    errorCode = -2;
    goto free_all_and_return;
error_opening_file:
    errorCode = -1;
    goto free_all_and_return;

free_all_and_return:
    freeAll(crewMembers, crewCount, rooms, roomCount);
    return errorCode;
}

int configSetAllRemainingFields(crewMember **crewMembers, size_t crewCount, room **rooms, size_t roomCount, char *configFilePath)
{
    enum currentlyChecking checking = CHECKING_NOTHING;

    char lineBuffer[MAX_LINE_LENGTH] = "";
    size_t id = 0;
    size_t roomIndex = -1;
    size_t crewIndex = -1;
    size_t lineNumber = 1;
    int errorCode = 0;

    FILE *fptr = fopen(configFilePath, "r");
    if (fptr == NULL)
        goto error_opening_file;

    while (fgets(lineBuffer, MAX_LINE_LENGTH, fptr) != NULL)
    {
        if (configLineStartsWith(lineBuffer, "room:")) {checking = CHECKING_ROOM; roomIndex++;}
        else if (configLineStartsWith(lineBuffer, "crewMember:")) {checking = CHECKING_CREWMEMBER; crewIndex++;}
        else if (checking == CHECKING_CREWMEMBER)
        {
            if (configLineStartsWith(lineBuffer, "  name:"))
                strncpy(crewMembers[crewIndex]->name, configGetStringAfterString(lineBuffer, "  name"), CREW_MEMBER_NAME_MAX_LENGTH);

            else if (configLineStartsWith(lineBuffer, "  job:"))
                crewMembers[crewIndex]->job = configGetIntAfterString(lineBuffer, "  job:") - 1;

            // TODO: refactor currentRooms to be in crewMember struct
            // else if (configLineStartsWith(lineBuffer, "  currentRoom:"))
            //     crewMembers[crewIndex]-> = configGetIntAfterString(lineBuffer, "  currentRoom:");
        }
        else if (checking == CHECKING_ROOM)
        {
            if (configLineStartsWith(lineBuffer, "  name:"))
                strncpy(rooms[roomIndex]->name, configGetStringAfterString(lineBuffer, "  name"), ROOM_NAME_MAX_LENGTH);

            else if (configLineStartsWith(lineBuffer, "  type:"))
                rooms[roomIndex]->roomType = configGetIntAfterString(lineBuffer, "  type:") - 1;

            else if (configLineStartsWith(lineBuffer, "  size:"))
                rooms[roomIndex]->size = configGetIntAfterString(lineBuffer, "  size:");

            else if (configLineStartsWith(lineBuffer, "  maxCrew:"))
                rooms[roomIndex]->crewCapacity = configGetIntAfterString(lineBuffer, "  maxCrew:");

            else if (configLineStartsWith(lineBuffer, "  storage:"))
                rooms[roomIndex]->storageCapacity = configGetIntAfterString(lineBuffer, "  storage:");
        }
        lineNumber++;
    }

    return 0;

error_opening_file:
    freeAll(crewMembers, crewCount, rooms, roomCount);
    return -1;
}

char *configGetStringAfterString(char *lineBuffer, char *startsWithString)
{
    int index = 0;
    char *actualStringStart;

    while (startsWithString[index] != '\0')
    {
        if (lineBuffer[index] != startsWithString[index])
            goto error_wrong_starts_with_string;        
        index++;
    }

    actualStringStart = lineBuffer + index;
    goToNextNonSpaceCharacterOnThisLine(&actualStringStart);

    char *endChar = actualStringStart;
    while (*endChar != '\0')
    {
        if (*endChar == '#')
            *endChar = '\0';
        else
            endChar++;
    }

    return actualStringStart;

error_wrong_starts_with_string:
    displayError("Error triggered in the function configGetIntAfterString() of config_parser.c !");
    return NULL;
}

/** checks if all the ids in the array as string of charPtr 
 * (looking like that : [+1,  -7   ,+8 ]) exist in roomArray
 */
int initAndCheckAdjacentRooms(room **roomArray, size_t roomArraySize, room *roomBeingChecked, char *charPtr, size_t lineNumber)
{
    char *lineBuffer = charPtr;
    char *separatorChar = charPtr;
    size_t adjacentRoomId = 0;
    size_t adjacentRoomsSize = 0;
    room **adjacentRooms = NULL;
    
    goToNextNonSpaceCharacterOnThisLine(&charPtr);
    if (*charPtr == ']')
        goto empty_list;

    adjacentRoomsSize = 1;
    while ((charPtr = getNextOccurenceOfCharOnThisLine(charPtr+1, ',')) != NULL)
        adjacentRoomsSize++;

    adjacentRooms = malloc(sizeof(room *) * adjacentRoomsSize);
    charPtr = lineBuffer;

    for (size_t i = 0; i < adjacentRoomsSize; i++)
    {
        if ((separatorChar = getNextOccurenceOfCharOnThisLine(separatorChar, ',')) != NULL)
            *separatorChar = '\0';

        room *adjacentRoom = configGetRoomInArray(roomArray, roomArraySize, (adjacentRoomId = configGetIntAfterString(charPtr, "")));
        if (adjacentRoom == NULL)
            goto id_does_not_exist;
        if (adjacentRoom->id == roomBeingChecked->id)
            goto room_adjacent_to_itself;

        adjacentRooms[i] = adjacentRoom;

        for (size_t j = 0; j < i; j++)
        {
            if (adjacentRooms[i]->id == adjacentRooms[j]->id)
                goto duplicate_id;
        }

        separatorChar++;
        charPtr = separatorChar;
    }

empty_list:
    roomBeingChecked->adjacentRoomsArraySize = adjacentRoomsSize;
    roomBeingChecked->adjacentRoomsArray = adjacentRooms;
    return 0;
id_does_not_exist:
    printf("Error : non-existing room id (%zu) used at line %zu\n", adjacentRoomId, lineNumber);
    return -1;
duplicate_id:
    printf("Error : duplicate room id (%zu) found at line %zu\n", adjacentRoomId, lineNumber);
    return -2;
room_adjacent_to_itself:
    printf("Error : A room cannot be adjacent to itself (id: %zu, line %zu)\n", adjacentRoomId, lineNumber);
    return -3;
}

int freeAll(crewMember **crewMembers, size_t crewCount, room **rooms, size_t roomCount)
{
    for (size_t i = 0; i < crewCount; i++)
        free(crewMembers[i]);

    free(crewMembers);
    crewMembers = NULL;

    for (size_t i = 0; i < roomCount; i++)
    {
        free(rooms[i]->adjacentRoomsArray);
        free(rooms[i]);
    }
    free(rooms);
    rooms = NULL;
    return 0;
}


room *configGetRoomInArray(room **roomsArray, size_t roomsArraySize, size_t roomIdToLookFor)
{
    for (size_t i = 0; i < roomsArraySize; i++)
    {
        if (roomsArray[i]->id == roomIdToLookFor)
            return roomsArray[i];
    }

    return NULL;
}

bool isRoomInArray(room **roomArray, size_t roomArraySize, size_t roomIdToLookFor)
{
    for (size_t i = 0; i < roomArraySize; i++)
        if (roomArray[i]->id == roomIdToLookFor)
            return true;

    return false;
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

hasDuplicatesIds(room);

hasDuplicatesIds(crewMember);

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
    displayError("Error triggered in the function configGetIntAfterString() of config_parser.c !");
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