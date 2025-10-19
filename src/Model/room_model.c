#include "room_model.h"

room **roomList = NULL;
size_t roomListSize = 0;

int addRoom(room *newRoom)
{
    room **newRoomList = malloc(sizeof(room*) * (roomListSize + 1));
    if (newRoomList == NULL) 
    {
        printf("ERROR: Failed to allocate enough memory to add a crewman to the list");
        return 1;
    }
    for (size_t i = 0; i < roomListSize; i++)
    {
        newRoomList[i] = roomList[i];
    }

    newRoomList[roomListSize] = newRoom;
    roomListSize++;

    free(roomList);

    roomList = newRoomList;
    return 0;
}

room* createRoom(
    char *name,
    enum roomType roomType,
    size_t crewCapacity,
    size_t storageCapacity,
    size_t size
)
{
    room *newRoom = malloc(sizeof(room));
    if (newRoom == NULL)
    {
        printf("ERROR: Failed to allocate memory for initial room");
        return NULL;
    }

    strncpy(newRoom->name, name, ROOM_NAME_SIZE);
    newRoom->roomType = roomType;
    newRoom->crewCapacity = crewCapacity;
    newRoom->storageCapacity = storageCapacity;
    newRoom->size = size;
    newRoom->adjacentRoomsArray = NULL;
    newRoom->adjacentRoomsArraySize = 0;

    return newRoom;
}

int addAdjacentRoom(room *roomToAddTo, room *adjacentRoomToAdd)
{
    if (isAdjacent(roomToAddTo, adjacentRoomToAdd))
    {
        return 0;
    }

    size_t amountOfAdjacentRooms = roomToAddTo->adjacentRoomsArraySize;
    room **newAdjacentRoomArray = malloc(sizeof(*(roomToAddTo->adjacentRoomsArray)) * (amountOfAdjacentRooms + 1));
    if (newAdjacentRoomArray == NULL) 
    {
        printf("ERROR: Failed to allocate enough memory to add an adjacent room to the list");
        return 1;
    }
    for (size_t i = 0; i < amountOfAdjacentRooms; i++)
    {        
        newAdjacentRoomArray[i] = roomToAddTo->adjacentRoomsArray[i];
    }

    
    newAdjacentRoomArray[amountOfAdjacentRooms] = adjacentRoomToAdd;
    roomToAddTo->adjacentRoomsArraySize++;

    free(roomToAddTo->adjacentRoomsArray);

    roomToAddTo->adjacentRoomsArray = newAdjacentRoomArray;

    addAdjacentRoom(adjacentRoomToAdd, roomToAddTo);
    return 0;
}

bool isAdjacent(room *referenceRoom, room *roomToLookFor)
{
    for (size_t i = 0; i < referenceRoom->adjacentRoomsArraySize; i++)
    {
        if (referenceRoom->adjacentRoomsArray[i] == roomToLookFor)
        {
            return true;
        }
    }

    return false;
}

int initRoom(int amountOfRooms)
{
    room *medbay = createRoom("Medbay 1", MEDBAY, 10, 7, 75);
    room *bridge = createRoom("Bridge", BRIDGE, 15, 3, 50);
    room *medbayToBridgeCorridor = createRoom("Corridor Medbay to Bridge", CORRIDOR, 20, 5, 100);

    addAdjacentRoom(medbay, medbayToBridgeCorridor);
    addAdjacentRoom(bridge, medbayToBridgeCorridor);

    addRoom(medbay);
    addRoom(bridge);
    addRoom(medbayToBridgeCorridor);

    return 0;
}