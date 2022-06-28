#define PID_MIN  	300
#define PID_MAX 	5000
#define ARR_SIZE    PID_MAX-PID_MIN+1

int pid_map[ARR_SIZE];

enum State {Free = 0, InUse = 1}; /* Enum to identify if pid is in use or free */


/**
 * Allocates the pid map. Creates and initializes a data structure for representing pids; 
 * returns -1 if unsuccessful, 1 if successful
 */
int allocate_map(void);

/**
 * Allocates a pid
 */
int allocate_pid(void);

/**
 * Releases a pid
 */
void release_pid(int pid);
