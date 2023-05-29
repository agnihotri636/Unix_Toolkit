//Author : Shivam Agnihotri
//Date : 1 Feb 2023

#include <iostream>
#include <unistd.h> //fork
#include <sys/wait.h> //waitpid
#include <sys/resource.h> //getrusage
#include <sys/time.h> //gettimeofday
#include <iomanip> //setprecision and fixed

using namespace std;

int main(int argc, char* argv[]) {
    if (argc<2) {

        //throw error if less than 2 arguments passed

        cerr<<"Usage: mytime.x cmd [arguments]"<<endl;
        return 1;
    }

    pid_t pid = fork();
    if (pid<0) {

        //error if fork returns negative value

        cerr<<"Error: fork has failed"<<endl;
        return 1;

    } else if (pid == 0) {
        
        // child process
        // executing command using execvp()
        execvp(argv[1], argv + 1);
        cerr<<"Error: execvp() has failed"<<endl;
        return 1;
    } else {
        // parent process
        int status;
        struct rusage usage;
        struct timeval start, end;

        //capture starting time of program
        gettimeofday(&start,NULL);
        //waiting for child process to finish execution
        waitpid(pid, &status,0);
        //capture end time of program
        gettimeofday(&end,NULL);
        //get resource usage for child
        getrusage(RUSAGE_CHILDREN, &usage);

        
        //user cpu time
        double user_time = usage.ru_utime.tv_sec+usage.ru_utime.tv_usec/1000000.0;
        
        //system cpu time
        double sys_time = usage.ru_stime.tv_sec+usage.ru_stime.tv_usec/1000000.0;
            
        //elapsed wall clock time
        double elapsed_time = (end.tv_sec-start.tv_sec)+(end.tv_usec-start.tv_usec)/1000000.0;


        // convert time to mins and seconds for cpu, system and wall clock

        int user_time_min = (int)user_time/60;
        double user_time_sec = user_time-user_time_min*60;

        int sys_time_min = (int)sys_time/60;
        double sys_time_sec = sys_time-sys_time_min*60;

        int elapsed_time_min = (int)elapsed_time/60;
        double elapsed_time_sec = elapsed_time-elapsed_time_min*60;


        //print results

        cout << "user CPU time\t" << user_time_min << "m" << fixed << setprecision(3) << user_time_sec << "s" << endl;
        cout << "system CPU time\t" << sys_time_min << "m" << fixed << setprecision(3) << sys_time_sec << "s" << endl;
        cout << "elapsed wall-clock time\t" << elapsed_time_min << "m" << fixed << setprecision(3) << elapsed_time_sec << "s" << endl;
    }

    return 0;
}
