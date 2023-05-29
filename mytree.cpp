//Author : Shivam Agnihotri
//Date : 1 Feb 2023


//including the necessary libraries and headers
#include <iostream>
#include <unistd.h> //getcwd function
#include <string>
#include <dirent.h> //directory functions opendir, readdir, closedir

using namespace std;

// creating function mytree with 2 parameters -
// directory path
// level will keep track current level of recursion

void mytree(string path, int level) {

    //opening directory using opendir

    DIR* dir;
    struct dirent* entry;
    if (!(dir = opendir(path.c_str())))
        return; //return if not able to open directory

    //reading the directory using readdir

    while ((entry = readdir(dir)) != NULL) {
        
        //unix hidden files start with a "."
        //skipping hidden files
        
        if (entry->d_name[0] == '.')
            continue;

        //generating directory path

        string full_path = path + "/" + entry->d_name;

        //printing indentation based on level
        
        for (int i = 0; i < level; i++) {
            cout << "|    ";
        }

        //printing directory with a "/"

        if (entry->d_type == DT_DIR) {
            cout << "| --- " << entry->d_name << "/" << endl;
        } else {
            cout << "| --- " << entry->d_name << endl;
        }

        //recurisively calling mytree on level+1 in case any entry is directory

        if (entry->d_type == DT_DIR) {
            mytree(full_path, level + 1);
        }
    }

    // close directory after all entries traversed

    closedir(dir);
}

int main(int argc, char* argv[]) {
    string path;
    if (argc == 1) {

        //in case no arguments passed used current working directory as path

        char cwd[256];
        getcwd(cwd, sizeof(cwd));
        path = cwd;
    } else {

        //passed directory as path

        path = argv[1];
    }
     cout<< path << endl;

    //call mytree function with path and initial level 0
         
    mytree(path, 0);

    return 0;
}

