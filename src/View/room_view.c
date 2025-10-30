#include "room_view.h"

int listRooms()
{
    printf("Rooms :\n");

    if (roomListSize == 0)
    {
        printf("There are no rooms.\n");
        return 0;
    }
    
    
    for (size_t i = 0; i < roomListSize; i++)
    {
        room *currentRoom = roomList[i];
        printf("<===============>\n");
        printRoomWithPrefix("", currentRoom);
        printf("Adjacent rooms (%d) :\n", currentRoom->adjacentRoomsArraySize);

        if (currentRoom->adjacentRoomsArraySize == 0)
        {
            printf("  No Adjacent Rooms\n");
        }

        room **adjacentRooms = currentRoom->adjacentRoomsArray;

        for (size_t j = 0; j < currentRoom->adjacentRoomsArraySize; j++)
        {
            printRoomWithPrefix("  - ", adjacentRooms[j]);
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
    fgets(name, nameLength, stdin);
    char *nameEndLineChar = strchr(name, '\n');
    if (nameEndLineChar != NULL)
        *nameEndLineChar = '\0';

    bool isRoomTypeSelected = false;
    do
    {        
        printf("Select a room type :\n");

        for (size_t i = 0; i < AMOUNT_OF_DIFFERENT_ROOM_TYPES; i++)
        {
            char roomTypeString[ROOM_TYPE_STRING_MAX_LENGTH] = "";
            getRoomTypeToString((enum roomType) i, roomTypeString, ROOM_TYPE_STRING_MAX_LENGTH);
            printf("  - %d : %s\n", i + 1, roomTypeString);
        }
 
        char *promptMessage = NULL;
        size_t roomTypeIndex = promptForSize_T(promptMessage);

        if (roomTypeIndex >= 1 && roomTypeIndex <= AMOUNT_OF_DIFFERENT_ROOM_TYPES)
        {
            *roomType = roomTypeIndex - 1;
            isRoomTypeSelected = true;
        }
    } while (!isRoomTypeSelected);

    *crewCapacity = promptForSize_T("Enter crew capacity :");
    
    *storageCapacity = promptForSize_T("Enter storage capacity :");
    
    *size = promptForSize_T("Enter size :");
 
    return 0;
}

int printRoomWithPrefix(char *prefix, room *room)
{
    char roomTypeString[ROOM_TYPE_STRING_MAX_LENGTH];
    getRoomTypeToString(room->roomType, roomTypeString, ROOM_TYPE_STRING_MAX_LENGTH);

    printf("%s%s (%s) [size : %zu, max crew : %zu, max storage : %zu] #%zu\n", prefix, room->name, roomTypeString, room->size, room->crewCapacity, room->storageCapacity, room->id);
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

size_t promptForSize_T(char *promptMessage)
{
    if (promptMessage != NULL)
        printf("%s\n", promptMessage);

    char userInput[256] = "";
    fgets(userInput, 256, stdin);
    
    char *endptr;
    size_t number = strtoul(userInput, &endptr, 0); 
    
    // I know I'm not doing any error handling here but it's
    // complicated + it doesn't cause too much problems so whatever

    return number; 
}