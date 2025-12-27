#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#define MAX_LINE_LENGTH 1000

#include <stdio.h>
#include <stdbool.h>
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

#endif // !CONFIG_PARSER_H