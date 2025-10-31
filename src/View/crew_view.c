#include "crew_view.h"

int listCrew() {
    printf("Crew : (" BLU BOLD "%zu" CRESET ")\n", crewListSize);

    if (crewListSize == 0)
    {
        printf(RED ITALIC "  The crew is empty." CRESET "\n");
        return 0;
    }
    
    
    for (size_t i = 0; i < crewListSize; i++)
    {
        char prefix[] = "  ";
        printCrewMemberWithPrefix(prefix, crewList[i]);
    }
    
    return 0;
}

int generateCrewMember(enum job *job, char *name, size_t nameLength)
{
    printf("Creating a crew member :\n");

    printf("Enter a name :\n");
    printf(GRN BOLD);
    fgets(name, nameLength, stdin);
    printf(CRESET);
    char *nameEndLineChar = strchr(name, '\n');
    if (nameEndLineChar != NULL)
        *nameEndLineChar = '\0';

    bool isJobSelected = false;
    do
    {        
        printf("Select a job :\n");

        for (size_t i = 0; i < AMOUNT_OF_DIFFERENT_JOBS; i++)
        {
            char jobString[JOB_STRING_MAX_LENGTH] = "";
            getJobToString((enum job) i, jobString, JOB_STRING_MAX_LENGTH);
            printf("  - %zu : %s\n", i + 1, jobString);
        }
        
        char *promptMessage = NULL;
        int jobIndex = promptForSize_T(promptMessage);

        if (jobIndex >= 1 && jobIndex <= AMOUNT_OF_DIFFERENT_JOBS)
        {
            *job = jobIndex - 1;
            isJobSelected = true;
        }

    } while (!isJobSelected);
 
    return 0;        
}

int viewRemoveCrewMember(size_t *crewMemberId)
{
    *crewMemberId = promptForSize_T("Enter the id of the crew member to remove :");
    return 0;
}

int handleRemoveCrewMemberErrors(int errorCode)
{
    switch (errorCode)
    {
        case 0:
            break;
        case -1:
            printf(RED BOLD "ERROR : Could not allocate enough memory to perform the operation" CRESET "\n");
            break;
        case -2:
            printf(GRN BOLD "There are no crew members" CRESET "\n");
            break;
        case -404:
            printf(GRN BOLD "The crew member could not be found" CRESET "\n");
            break;
        
        default:
            printf(RED BOLD "Undefined error code \"%d\" in the handleRemoveCrewMemberErrors function. What the fuck?" CRESET "\n");
            return -1;
            break;
    }

    return 0;
}

int getJobToString(enum job job, char *jobString, int jobStringMaxSize) {
    switch (job)
    {
        case ENGINEER:
            strncpy(jobString, "Engineer", jobStringMaxSize);
            break;
        
        default:
            strncpy(jobString, "Unregistered Job", jobStringMaxSize);
            break;
    }

    return 0;
}

int printCrewMemberWithPrefix(char *prefix, crewMember *crewMemberToDisplay)
{
    char jobString[JOB_STRING_MAX_LENGTH] = "";
    getJobToString(crewMemberToDisplay->job, jobString, JOB_STRING_MAX_LENGTH); // jobString gets turned into a char pointer when passed

    printf("%s%s (" RED "%s" CRESET ") " YEL BOLD "#%zu" CRESET "\n", prefix, crewMemberToDisplay->name, jobString, crewMemberToDisplay->id);
}