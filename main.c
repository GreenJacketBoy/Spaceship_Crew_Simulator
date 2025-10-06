#include <stdio.h>
#include <string.h>

int handleCmdInput(char* cmd)
{
    if (strcmp(cmd, "crew ls\n") == 0)
    {
        printf("Crew List !\n");
        return 1;
    }
    
    return -1;
}

int main() 
{
    while (1 == 1)
    {
        char cmd[20];
        printf("Enter a command :\n");
        fgets(cmd, 20, stdin); 

        printf("You've entered : %s\n", cmd);

        char* cmdPtr = cmd;
        
        handleCmdInput(cmdPtr);
    }
    
}