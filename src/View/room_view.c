#include "room_view.h"

int listRooms()
{
    printf("Rooms (" BLU BOLD "%zu" CRESET "):\n", roomListSize);

    if (roomListSize == 0)
    {
        printf("There are no rooms.\n");
        return 0;
    }
    
    
    for (size_t i = 0; i < roomListSize; i++)
    {
        room *currentRoom = roomList[i];
        printf(YEL BOLD "<===============>\n" CRESET);
        printRoomWithPrefix("", currentRoom);
        printf("Adjacent rooms (" BLU BOLD "%d" CRESET ") :\n", currentRoom->adjacentRoomsArraySize);

        if (currentRoom->adjacentRoomsArraySize == 0)
        {
            printf("  " RED ITALIC "No Adjacent Rooms" CRESET "\n");
        }

        room **adjacentRooms = currentRoom->adjacentRoomsArray;

        for (size_t j = 0; j < currentRoom->adjacentRoomsArraySize; j++)
        {
            printRoomWithPrefix(RED BOLD "  - " CRESET, adjacentRooms[j]);
        }
    
    }

    return 0;
}

int promptRoomAttributes(
    char *name,
    size_t nameLength,
    enum roomType *roomType,
    size_t *crewCapacity,
    size_t *storageCapacity,
    size_t *size
)
{
    printf("Creating a new room :\n");

    printf("Enter a name :\n");
    if (promptForString(name, nameLength) != 0)
        goto error_reading_input;

    char *nameEndLineChar = strchr(name, '\n');
    if (nameEndLineChar != NULL)
        *nameEndLineChar = '\0';

    *roomType = promptForRoomType("Select a room type :");

    *crewCapacity = promptForSize_T("Enter crew capacity :");
    
    *storageCapacity = promptForSize_T("Enter storage capacity :");
    
    *size = promptForSize_T("Enter size :");
 
    return 0;

error_reading_input:
    printf(CRESET);
    return -1;
}

int viewRoomRm(size_t *roomIdToRemove)
{
    *roomIdToRemove = promptForSize_T("Enter the Id of the room to remove :");
    return 0;
}

int viewRoomEdit(
    room *roomToEdit,
    char *newName,
    size_t newNameLength,
    enum roomType *newType,
    size_t *newCrewCapacity,
    size_t *newStorageCapacity,
    size_t *newSize
) // Most of the code is copied from promptRoomAttributes()
{
    printf("Editing a room :\n");

    printf("Enter a new name (previously " GRN "%s" CRESET ") :\n", roomToEdit->name);
    if (promptForString(newName, newNameLength) != 0)
        goto error_reading_input;

    char *nameEndLineChar = strchr(newName, '\n');
    if (nameEndLineChar != NULL)
        *nameEndLineChar = '\0';

    bool isRoomTypeSelected = false;

    char previousRoomTypeString[ROOM_TYPE_STRING_MAX_LENGTH] = "";
    getRoomTypeToString(roomToEdit->roomType, previousRoomTypeString, ROOM_TYPE_STRING_MAX_LENGTH);

    char promptMessage[128 + ROOM_TYPE_STRING_MAX_LENGTH] = "";
    snprintf(
        promptMessage,
        128 + ROOM_TYPE_STRING_MAX_LENGTH,
        "Select a new job (previously " GRN "%s" CRESET " (" BLU "%u" CRESET ")) :",
        previousRoomTypeString,
        roomToEdit->roomType + 1
    );

    *newType = promptForRoomType(promptMessage);

    char crewCapacityPromptMessage[128] = "Error!";
    snprintf(crewCapacityPromptMessage, 128, "Enter the new crew capacity (previously " BLU "%zu" CRESET ") :", roomToEdit->crewCapacity);
    *newCrewCapacity = promptForSize_T(crewCapacityPromptMessage);
 
    char storageCapacityPromptMessage[128] = "Error!";
    snprintf(storageCapacityPromptMessage, 128, "Enter the new storage capacity (previously " BLU "%zu" CRESET ") :", roomToEdit->storageCapacity);
    *newStorageCapacity = promptForSize_T(storageCapacityPromptMessage);
 
    char sizePromptMessage[128] = "Error!";
    snprintf(sizePromptMessage, 128, "Enter the new room size (previously " BLU "%zu" CRESET ") :", roomToEdit->size);
    *newSize = promptForSize_T(sizePromptMessage);
 
    return 0;

error_reading_input:
    printf(CRESET);
    return -1;
}


int printRoomWithPrefix(char *prefix, room *room)
{
    char roomTypeString[ROOM_TYPE_STRING_MAX_LENGTH];
    getRoomTypeToString(room->roomType, roomTypeString, ROOM_TYPE_STRING_MAX_LENGTH);

    printf("%s%s (" GRN "%s" CRESET ") " RED "[" CRESET "size : " BLU "%zu" CRESET ", max crew : " BLU "%zu" CRESET ", max storage : " BLU "%zu" CRESET "" RED "]" CRESET "" YEL BOLD " #%zu" CRESET "\n", prefix, room->name, roomTypeString, room->size, room->crewCapacity, room->storageCapacity, room->id);
    return 0;
}

int getRoomTypeToString(enum roomType roomType, char *roomTypeString, size_t roomTypeStringMaxLength)
{
    switch (roomType)
    {
        case CORRIDOR:
            strncpy(roomTypeString, "Corridor", roomTypeStringMaxLength);
            break;
        case MEDBAY:
            strncpy(roomTypeString, "Medbay", roomTypeStringMaxLength);
            break;
        case QUARTER:
            strncpy(roomTypeString, "Quarter", roomTypeStringMaxLength);
            break;
        case BRIDGE:
            strncpy(roomTypeString, "Bridge", roomTypeStringMaxLength);
            break;
        
        default:
            strncpy(roomTypeString, "Unregistered Room Type", roomTypeStringMaxLength);
            break;
    }

    return 0;
}

enum roomType promptForRoomType(char *promptMessage)
{
    enum roomType returnRoomType = 0;
    bool isRoomTypeSelected = false;

    while (!isRoomTypeSelected)
    {
        printf("%s\n", promptMessage);

        for (size_t i = 0; i < AMOUNT_OF_DIFFERENT_ROOM_TYPES; i++)
        {
            char roomTypeString[ROOM_TYPE_STRING_MAX_LENGTH] = "";
            getRoomTypeToString((enum roomType) i, roomTypeString, ROOM_TYPE_STRING_MAX_LENGTH);
            printf("  - " BLU "%zu" CRESET " : " GRN "%s" CRESET "\n", i + 1, roomTypeString);
        }
 
        char *promptMessage = NULL;
        size_t roomTypeIndex = promptForSize_T(promptMessage);

        if (roomTypeIndex >= 1 && roomTypeIndex <= AMOUNT_OF_DIFFERENT_ROOM_TYPES)
        {
            returnRoomType = roomTypeIndex - 1;
            isRoomTypeSelected = true;
        }
    } 

    return returnRoomType;
}