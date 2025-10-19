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

int printRoomWithPrefix(char *prefix, room *room)
{
    char roomTypeString[ROOM_TYPE_STRING_MAX_LENGTH];
    getRoomTypeToString(room->roomType, roomTypeString, ROOM_TYPE_STRING_MAX_LENGTH);

    printf("%s%s (%s)\n", prefix, room->name, roomTypeString);
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