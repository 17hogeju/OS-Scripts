/**
 *
 * Operating System Concepts
 * Copyright John Wiley & Sons, 2013.
 */

#include "pid.h"
#include <pthread.h>
#include <stdio.h>

int allocate_map(void) 
{
    int i;

    for(i = 0; i < ARR_SIZE; i++)
    {
        pid_map[i] = Free;
        if(pid_map[i] != Free){ /* If any assignment did not work, return -1 */
            return -1;
        }
    }
    return 1; /* pid map initialized without any errors */
}

int allocate_pid(void)
{
    int i;
    for(i = 0; i < ARR_SIZE; i++) /* Go through each pid in map to find a free one */
    {
        if(pid_map[i] == Free) /* If pid is free, mark it as in use and return the pid number */
        {
            pid_map[i] = InUse;
            return i + PID_MIN;
        }
    }
    return -1; /* If there are no available pids */
}

void release_pid(int pid) /* Find the corresponding array value and mark it as free */
{
    int arr_location = pid - PID_MIN;
    pid_map[arr_location] = Free;
}

