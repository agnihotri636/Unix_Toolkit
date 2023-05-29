//Author : Shivam Agnihotri
//Date : 1 Feb 2023

#include <iostream>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>

using namespace std;

const int NUM_HRS = 24;
const int TIME_WINDOW_IN_SECONDS=NUM_HRS*3600;

int main(int argc, char* argv[]) {
    // creating file_cnt array for storing number of files modified every hour
    // initialize to 0

    int file_cnt[NUM_HRS] = { 0 };

    // return current time from time function
    time_t current_time = time(0);
    

    // if directory not given then take directory as current working directory
    string dir = (argc > 1) ? argv[1] : ".";

    // open directory
    DIR* dp = opendir(dir.c_str());
    if (dp == NULL) {
        cerr<<"Error: Unable to open directory "<<dir<<endl;
        return 1;
    }

    // reading directory content
    struct dirent* entry;
    while ((entry = readdir(dp)) != NULL) {
        
        // generate full path of file
        string file_path = dir + "/" + entry->d_name;

        // get file information
        struct stat file_info;
        if (stat(file_path.c_str(), &file_info) != 0) {
            cerr << "Error: Unable to get file info for the path " << file_path << endl;
            continue;
        }

        // check if regular file
        if (!S_ISREG(file_info.st_mode)) {
            continue;
        }

        // check for modification in last 24 hrs
        if (difftime(current_time, file_info.st_mtime) > TIME_WINDOW_IN_SECONDS) {
            continue;
        }
        
        // Get the hour of the last modification time

        int hour = difftime(current_time, file_info.st_mtime) / 3600;
        
        // increment count for the respective hour
        file_cnt[hour]++;
    }

    // close directory
    closedir(dp);

    // printing result
    char date_time[100];
    for (int i = NUM_HRS; i > 0; i--){
            time_t current_window_time=current_time - i*3600;

            struct tm* last_mod_time = localtime(&current_window_time);
                    

            strftime(date_time, sizeof(date_time), "%a %b %e %H:%M:%S %Y: ", last_mod_time);
            cout<<date_time<<file_cnt[i-1]<<endl;
        
    }
        //cout<<file_cnt[0]<<endl;
        //cout<<file_cnt[1]<<endl;
    return 0;
}
