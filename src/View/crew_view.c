#include "crew_view.h"

int listCrew() {
    printf("Crew :\n");

    if (crewListSize == 0)
    {
        printf("The crew is empty.\n");
        return 0;
    }
    
    
    for (size_t i = 0; i < crewListSize; i++)
    {
        printf("Name: %s\n", crewList[i]->name);

        char jobString[JOB_STRING_MAX_LENGTH] = "";
        getJobToString(crewList[i]->job, jobString, JOB_STRING_MAX_LENGTH); // jobString gets turned into a char pointer when passed

        printf("Job: %s\n", jobString);
    }
    
    return 0;
}

int generateCrewMember(enum job *job, char *name, size_t nameLength)
{
    printf("Creating a crew member :\n");

    printf("Enter a name :\n");
    fgets(name, nameLength, stdin);

    bool isJobSelected = false;
    do
    {        
        printf("Select a job :\n");

        for (size_t i = 0; i < AMOUNT_OF_DIFFERENT_JOBS; i++)
        {
            char jobString[JOB_STRING_MAX_LENGTH] = "";
            getJobToString((enum job) i, jobString, JOB_STRING_MAX_LENGTH);
            printf("  - %c : %s\n", ('a' + i), jobString);
        }
        
        char userInputJob[64] = "";
        fgets(userInputJob, 64, stdin);
        int jobIndex = userInputJob[0] - 'a'; // I came up with it close to all by myself ^^

        if (jobIndex >= 0 && jobIndex < AMOUNT_OF_DIFFERENT_JOBS)
        {
            *job = jobIndex;
            isJobSelected = true;
        }

    } while (!isJobSelected);
 
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