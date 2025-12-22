#include "crew_view.h"

int listCrew(crewMember **crewList, size_t crewListSize) {
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
    if (promptForString(name, nameLength) != 0)
        goto error_reading_input;

    *job = promptForJob("Select a job :");

 
    return 0;        

error_reading_input:
    return -1;
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

// code was mostly copied from generateCrewMember()
int viewEditCrewMember(crewMember *crewMemberToEdit, enum job *newJob, char *newName, size_t nameLength)
{
    printf("Editing a crew member :\n");

    printf("Enter a new name (previously " GRN "%s" CRESET "):\n", crewMemberToEdit->name);
    if (promptForString(newName, nameLength) != 0)
        goto error_reading_input;

    bool isJobSelected = false;

    char previousJobString[JOB_STRING_MAX_LENGTH] = "";
    getJobToString(crewMemberToEdit->job, previousJobString, JOB_STRING_MAX_LENGTH);

    char promptMessage[128 + JOB_STRING_MAX_LENGTH] = "";
    snprintf(
        promptMessage,
        128 + JOB_STRING_MAX_LENGTH,
        "Select a new job (previously " GRN "%s" CRESET " (" BLU "%u" CRESET ")) :",
        previousJobString,
        crewMemberToEdit->job + 1
    );

    *newJob = promptForJob(promptMessage);

    return 0;        

error_reading_input:
    return -1;
}

int viewShowCrewMember(crewRoomLink *crewRoomLinkToDisplay)
{
    crewMember *crewMemberToDiplay = crewRoomLinkToDisplay->crewMember;
    room *currentRoom = crewRoomLinkToDisplay->currentRoom;
    room *destinationRoom = crewRoomLinkToDisplay->destinationRoom;

    char jobString[JOB_STRING_MAX_LENGTH] = "";
    getJobToString(crewMemberToDiplay->job, jobString, JOB_STRING_MAX_LENGTH);

    printf("Crew member's details :\n");
    printf("- " BOLD "Id:   " CRESET YEL BOLD "#%zu" CRESET "\n", crewMemberToDiplay->id);
    printf("- " BOLD "Name: " CRESET GRN BOLD "%s" CRESET "\n", crewMemberToDiplay->name);
    printf("- " BOLD "Job:  " CRESET BLU "%s" CRESET "\n", jobString);
    if (currentRoom == NULL)
        printf("- Current location:    " RED BOLD "Not found" CRESET "\n");
    else
        printf("- Current location:    " YEL BOLD "#%zu " BLU BOLD "%s" CRESET "\n", currentRoom->id, currentRoom->name);

    if (destinationRoom == NULL)
        printf("- Current destination: " RED BOLD "Not found" CRESET "\n");
    else
        printf("- Current destination: " YEL BOLD "#%zu " BLU BOLD "%s" CRESET "\n", destinationRoom->id, destinationRoom->name);

    return 0;
}


int getJobToString(enum job job, char *jobString, int jobStringMaxSize) {
    switch (job)
    {
        case ENGINEER:
            strncpy(jobString, "Engineer", jobStringMaxSize);
            break;

        case PILOT:
            strncpy(jobString, "Pilot", jobStringMaxSize);
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

enum job promptForJob(char *promptMessage)
{
    enum job returnedJob = 0;
    bool isJobSelected = false;

    while (!isJobSelected)
    {        
        printf("%s\n", promptMessage);

        for (size_t i = 0; i < AMOUNT_OF_DIFFERENT_JOBS; i++)
        {
            char jobString[JOB_STRING_MAX_LENGTH] = "";
            getJobToString((enum job) i, jobString, JOB_STRING_MAX_LENGTH);
            printf("  - " BLU "%zu" CRESET " : " GRN "%s" CRESET "\n", i + 1, jobString);
        }
        
        char *jobIndexPromptMessage = NULL;
        int jobIndex = promptForSize_T(jobIndexPromptMessage);

        if (jobIndex >= 1 && jobIndex <= AMOUNT_OF_DIFFERENT_JOBS)
        {
            returnedJob = jobIndex - 1;
            isJobSelected = true;
        }
    }

    return returnedJob;
}