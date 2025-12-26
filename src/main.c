#include "main.h"

int main() 
{
    configCheckIntegrity();
    initCrew(4);
    initRoom(0);
    
    while (1 == 1)
    {
        promptAndRedirect();
    }
}