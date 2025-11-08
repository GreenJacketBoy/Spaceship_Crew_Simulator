#include "room_model.h"

size_t currentBiggestRoomId = 0;
room **roomList = NULL;
size_t roomListSize = 0;

int addRoom(room *newRoom)
{
    room **newRoomList = malloc(sizeof(room*) * (roomListSize + 1));
    if (newRoomList == NULL) 
    {
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
        return NULL;
 
    currentBiggestRoomId++;

    newRoom->id = currentBiggestRoomId;
    strncpy(newRoom->name, name, ROOM_NAME_MAX_LENGTH);
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
        return 0;


    size_t amountOfAdjacentRooms = roomToAddTo->adjacentRoomsArraySize;
    room **newAdjacentRoomArray = malloc(sizeof(*(roomToAddTo->adjacentRoomsArray)) * (amountOfAdjacentRooms + 1));
    if (newAdjacentRoomArray == NULL) 
        goto error_malloc_faillure;
 
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

error_malloc_faillure:
    displayError("Failed to allocate enough memory to add an adjacent room to the list");
    return 1;
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

int destroyRoom(size_t roomId)
{
    bool deepRemoval = true;
    room *removedRoom = popRoomFromRoomArray(&roomList, &roomListSize, roomId, deepRemoval);
    
    if (removedRoom == NULL)
        return -1;

    free(removedRoom);
    return 0;
}

room *popRoomFromRoomArray(room ***roomsArray, size_t *roomsArraySize, size_t roomIdToRemove, bool deepRemoval)
{
    room **newRoomArray = NULL;
    room *poppedRoom = NULL;
    
    if (*roomsArraySize == 0)
        goto error;

    newRoomArray = malloc(sizeof(room*) * (*roomsArraySize - 1));

    bool roomWasFound = false;
    for (size_t i = 0; i < *roomsArraySize; i++)
    {
        if (roomWasFound)
            newRoomArray[i - 1] = (*roomsArray)[i]; // -1 to account for the removed room
        else if ((*roomsArray)[i]->id == roomIdToRemove)
        {
            if (deepRemoval)
            {
                for (size_t j = 0; j < (*roomsArray)[i]->adjacentRoomsArraySize; j++)
                {
                    popRoomFromRoomArray(
                        &((*roomsArray)[i]->adjacentRoomsArray[j]->adjacentRoomsArray),
                        &((*roomsArray)[i]->adjacentRoomsArray[j]->adjacentRoomsArraySize),
                        roomIdToRemove,
                        false
                    );
                }
            }
 
            poppedRoom = (*roomsArray)[i];
            roomWasFound = true;

        }
        else if (i >= *roomsArraySize - 1)
            goto error;
        else
            newRoomArray[i] = (*roomsArray)[i]; 
    }

    free(*roomsArray);
    *roomsArray = newRoomArray;
    *roomsArraySize = *roomsArraySize - 1;

    return poppedRoom;
    
error:
    free(newRoomArray);
    return NULL;
}