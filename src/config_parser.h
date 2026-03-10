#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#define MAX_LINE_LENGTH 1000
#define CONFIG_FILE_NAME "config.yml"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "core.h"
#include "Model/type_definitions.h"
#include "Model/room_model.h"

enum currentlyChecking 
{
    CHECKING_NOTHING,
    CHECKING_ROOM,
    CHECKING_CREWMEMBER,
};

int configCheckIntegrityAndFillParams(crewMember ***crewMembers, size_t *crewCount, room ***rooms, size_t *roomCount, size_t *currentBiggestRoomId, size_t *currentBiggestCrewMemberId);
int configCheckIntegrityAttributesAreTheCorrectType(char *configFilePath);
int configCheckIntegrityAllFieldsPresentForAllObjects(char *configFilePath);
bool configLineStartsWith(char *cptr, char *stringToMatch);
bool isRestOfTheLineEmpty(char *cptr);
bool configCheckIntegrityCrewMember(char *lineBuffer, size_t *lineNumber, FILE *fptr);
bool configCheckIntegrityRoom(char *lineBuffer, size_t *lineNumber, FILE *fptr);
int assertStartsWithAndIsExpectedType(char *cptr, enum configType expectedType, char *startsWithString);
bool isCharNumber(char charToTest);
int goToNextNonSpaceCharacterOnThisLine(char **cptr);
bool isNumberUntilChar(char *cptr, char endChar);
char *getNextOccurenceOfCharOnThisLine(char *cptr, char charToLookFor);
bool isArrayOfNumbersUntilChar(char *cptr, char endChar);
int configCheckIntegrityNoDuplicateIds(crewMember ***crewMembers, size_t *crewCount, room ***rooms, size_t *roomCount, size_t *currentBiggestRoomId, size_t *currentBiggestCrewMemberId, char *configFilePath);
int countRoomsAndCrew(size_t *roomCount, size_t *crewCount, char *configFilePath);
bool hasDuplicates(size_t *arrayToCheck, size_t *lineNumbersArray, size_t size);
size_t configGetIntAfterString(char *cptr, char *startsWithString);
bool isRoomInArray(room **roomArray, size_t roomArraySize, size_t roomIdToLookFor);
room *configGetRoomInArray(room **roomsArray, size_t roomsArraySize, size_t roomIdToLookFor);
int initAndCheckAdjacentRooms(room **roomArray, size_t roomArraySize, room *roomBeingChecked, char *charPtr, size_t lineNumber);
int freeAll(crewMember **crewMembers, size_t crewCount, room **rooms, size_t roomCount);
int configCheckIntegrityReferencedIdsExist(crewMember **crewMembers, size_t crewCount, room **rooms, size_t roomCount, char *configFilePath);
char *configGetStringAfterString(char *lineBuffer, char *startsWithString);
int configSetAllRemainingFields(crewMember **crewMembers, size_t crewCount, room **rooms, size_t roomCount, char *configFilePath);

/** room_or_crewMember sould be either room or crewmember, nothing else */
#define hasDuplicatesIds(room_or_crewMember)\
bool room_or_crewMember##HasDuplicatesIds(room_or_crewMember **arrayToCheck, size_t *lineNumbersArray, size_t size)\
{                                                                                                                 \
    size_t firstDuplicateIndex, secondDuplicateIndex;                                                             \
    for (size_t i = 0; i < size; i++)                                                                             \
    {                                                                                                             \
        int instancesFound = 0;                                                                                   \
        for (size_t j = 0; j < size; j++)                                                                         \
        {                                                                                                         \
            if (arrayToCheck[i]->id == arrayToCheck[j]->id)                                                         \
            {                                                                                                     \
                instancesFound++;                                                                                 \
                if (instancesFound > 1)                                                                           \
                {                                                                                                 \
                    firstDuplicateIndex = lineNumbersArray[i];                                                    \
                    secondDuplicateIndex = lineNumbersArray[j];                                                   \
                    goto duplicate_found;                                                                         \
                }                                                                                                 \
            }                                                                                                     \
        }                                                                                                         \
    }                                                                                                             \
    return false;                                                                                                 \
duplicate_found:                                                                                                  \
    printf("Error : non-distinct " #room_or_crewMember " ids at line %zu and %zu\n", firstDuplicateIndex, secondDuplicateIndex);\
    return true;                                                                                                  \
}                                                                                                                 \

bool roomHasDuplicatesIds(room **arrayToCheck, size_t *lineNumbersArray, size_t size);
bool crewMemberHasDuplicatesIds(crewMember **arrayToCheck, size_t *lineNumbersArray, size_t size);

#endif // !CONFIG_PARSER_H