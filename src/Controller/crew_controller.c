#include "crew_controller.h"

int main() 
{
    initCrew(4);
    
    while (1 == 1)
    {
        char cmd[64];
        printf("Enter a command :\n");
        fgets(cmd, sizeof(cmd), stdin); 

        printf("You've entered : %s\n", cmd);

        char* cmdPtr = cmd;
        
        handleCmdInput(cmdPtr);
    }
}


int handleCmdInput(char* cmd)
{
    if (strcmp(cmd, "crew ls\n") == 0)
    {
        listCrew();
        return 0;
    }
    
    return -1;
}
