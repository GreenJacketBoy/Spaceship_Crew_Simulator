#include "crew_room_linker_view.h"

int displayCrewRoomLinker(crewRoomLink **linkArray, size_t linkArraySize)
{
    if (linkArraySize == 0)
    {
        printf(RED ITALIC "There are no Crew Room Links" CRESET "\n");
        return 0;
    }

    for (size_t i = 0; i < linkArraySize; i++)
    {

        crewMember *currentCrewMember = linkArray[i]->crewMember;
        room *currentRoom = linkArray[i]->currentRoom;
        room *destinationRoom = linkArray[i]->destinationRoom;

        printf(
            GRN BOLD "  %s" CRESET " " YEL BOLD "#%zu" CRESET ", currently " BLU BOLD "%s" CRESET ", headed "  BLU BOLD "%s" CRESET "\n"
            ,
            currentCrewMember->name,
            currentCrewMember->id,
            currentRoom == NULL ? RED BOLD "nowhere" : currentRoom->name,
            destinationRoom == NULL ? RED BOLD "nowhere" : destinationRoom->name
        );
    }

    return 0;
}