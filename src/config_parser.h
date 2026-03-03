#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#define MAX_LINE_LENGTH 1000
#define CONFIG_FILE_NAME "config.yml"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "core.h"
#include "Model/type_definitions.h"

int configCheckIntegrity();
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
int configCheckIntegrityNoDuplicateIds(char *configFilePath);
int countRoomsAndCrew(size_t *roomCount, size_t *crewCount, char *configFilePath);
bool hasDuplicates(size_t *arrayToCheck, size_t *lineNumbersArray, size_t size);
size_t configGetIntAfterString(char *cptr, char *startsWithString);

#endif // !CONFIG_PARSER_H