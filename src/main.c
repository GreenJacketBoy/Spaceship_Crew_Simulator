#include "main.h"

int main() 
{
    // TODO: Fill with arguments
    configCheckIntegrityAndFillParams(&crewList, &crewListSize, &roomList, &roomListSize, &currentBiggestRoomId, &currentBiggestCrewMemberId);
    
    while (1 == 1)
    {
        promptAndRedirect();
    }
}