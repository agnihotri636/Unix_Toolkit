//Author : Shivam Agnihotri
//Date : 6 Mar 2023


//including the necessary libraries and headers

#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/time.h>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Usage: mytimeout.x <time> <command> [args...]" << endl;
        return 1;
    }
    
    int time_limit = atoi(argv[1]);
    char** command_args = &argv[2];
    
    pid_t pid = fork();
    
    if (pid < 0) {
        cerr << "Failed to fork process." << endl;
        return 1;
    }
    else if (pid == 0) {
        // Child process
        execvp(command_args[0], command_args);
        cerr << "Failed to execute command: " << command_args[0] << endl;
        return 1;
    }
    else {

        struct timeval start_time, end_time;
        gettimeofday(&start_time, NULL);
        
        while (true) {
            int status;
            pid_t result = waitpid(pid, &status, WNOHANG);
            
            if (result == -1) {
                cerr << "Error waiting for child process." << endl;
                return 1;
            }
            
            if (result == pid) {
                // Child process has terminated
                if (WIFEXITED(status)) {
                    //Child process exited normally
                    cout << "Command exited successfully with status " << WEXITSTATUS(status) << endl;
                    return WEXITSTATUS(status);

                } else {
                    // Child process terminated abnormally
                     cout << "Command terminated by signal " << WTERMSIG(status) << endl;
                    return 1;
                }
            }
            
            gettimeofday(&end_time, NULL);
            double elapsed_time = (end_time.tv_sec - start_time.tv_sec) +
                                  (end_time.tv_usec - start_time.tv_usec) / 1000000.0;
            if (elapsed_time > time_limit) {
                // Timeout reached, kill the child process
                kill(pid, SIGTERM);
                 // Time limit reached
         if (kill(pid, SIGTERM) == 0) {
            cout << "Command terminated due to timeout" << endl;
         } else {
            cout << "Failed to terminate command ";
         }

                return 1;
            }
        }
    }
}
