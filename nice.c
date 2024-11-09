#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(int argc, char *argv[]) {
    int priority, pid;

    // If one argument is passed, assume it's the new priority for the current process
    if (argc == 2) {
        pid = getpid();  // Get the PID of the current process
        priority = atoi(argv[1]);
    } 
    // If two arguments are passed, the first is the PID, and the second is the priority
    else if (argc == 3) {
        pid = atoi(argv[1]);
        priority = atoi(argv[2]);
    } 
    else {
        // Print usage message if incorrect number of arguments
        printf(2, "Usage: nice pid priority\n");
        exit();
    }

    // Check if the priority is within the valid range
    if (priority < 0 || priority > 20) {
        printf(2, "Invalid priority (0-20)!\n");
        exit();
    }

    // Call the system call to change the nice value
    chpr(pid, priority);
    exit();
}

