#include "main.h"

int main() 
{
    // TODO: Fill with arguments
    configCheckIntegrityAndFillParams();
    initCrew(4);
    initRoom(0);
    
    while (1 == 1)
    {
        promptAndRedirect();
    }
}