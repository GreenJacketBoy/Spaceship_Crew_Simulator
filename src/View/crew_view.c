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

        int jobStringMaxSize = 64;
        char jobString[jobStringMaxSize];
        getJobToString(crewList[i]->job, jobString, &jobStringMaxSize); // jobString gets turned into a char pointer when passed

        printf("Job: %s\n", jobString);
    }
    
    return 0;
}

int getJobToString(enum job job, char *jobString, int *jobStringMaxSize) {
    switch (job)
    {
        case ENGINEER:
            strncpy(jobString, "Engineer", *jobStringMaxSize);
            break;
        
        default:
            strncpy(jobString, "Unregistered Job", *jobStringMaxSize);
            break;
    }

    return 0;
}