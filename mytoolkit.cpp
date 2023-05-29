//Author : Shivam Agnihotri
//Date : 6  Mar 2023


//including the necessary libraries and headers

#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <sys/wait.h>

using namespace std;

//print prompt
void printPrompt() {
    cout << "$ ";
}

//parse command and store in a string of vectors
vector<string> parse_command(const string& command) {
    vector<string> args;
    string arg;
    for (char c : command) {
        if (c == ' ') {
            if (!arg.empty()) {
                args.push_back(arg);
                arg.clear();
            }
        } else {
            arg += c;
        }
    }
    if (!arg.empty()) {
        args.push_back(arg);
    }
    return args;
}

// execute a single command with optional input/output redirection
void execute_command(const vector<string>& args, const string& input_file, const string& output_file) {
    //Convert the vector of arguments to an array of C strings
    int argc = args.size();
    char* argv[argc+1];
    for (int i = 0; i < argc; i++) {
        argv[i] = const_cast<char*>(args[i].c_str());
    }
    argv[argc] = nullptr;

    //Redirect input/output if specified
    if (!input_file.empty()) {
        int fd = open(input_file.c_str(), O_RDONLY);
        dup2(fd, STDIN_FILENO);
        close(fd);
    }
    if (!output_file.empty()) {
        int fd = open(output_file.c_str(), O_WRONLY|O_CREAT|O_TRUNC, 0666);
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }

    //Execute the command
    execvp(argv[0], argv);
    perror("execvp");
    exit(EXIT_FAILURE);
}

// Function to execute a pipeline of commands with optional input/output redirections
void execute_pipeline(const vector<vector<string>>& commands, const string& input_file, const string& output_file) {
    // Create pipes for inter-process communication
    int num_commands = commands.size();
    int pipefds[num_commands-1][2];
    for (int i = 0; i < num_commands-1; i++) {
        if (pipe(pipefds[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    //Execute all commands
    pid_t pids[num_commands];
    for (int i = 0; i < num_commands; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Child process
            // Redirect input/output as necessary
            if (i == 0 && !input_file.empty()) {
                int fd = open(input_file.c_str(), O_RDONLY);
                dup2(fd, STDIN_FILENO);
                close(fd);
            }
            if (i == num_commands-1 && !output_file.empty()) {
                int fd = open(output_file.c_str(), O_WRONLY|O_CREAT|O_TRUNC, 0666);
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }
            if (i > 0) {
                dup2(pipefds[i-1][0], STDIN_FILENO);
                close(pipefds[i-1][0]);
                close(pipefds[i-1][1]);
            }
            if (i < num_commands-1) {
                close(pipefds[i][0]);
            dup2(pipefds[i][1], STDOUT_FILENO);
            close(pipefds[i][0]);
            close(pipefds[i][1]);
        }

        execute_command(commands[i], input_file, output_file);
    } else {
        // Parent process
        pids[i] = pid;
        if (i > 0) {
            close(pipefds[i-1][0]);
            close(pipefds[i-1][1]);
        }
    }
}

//Wait for child to finish
for (int i = 0; i < num_commands; i++) {
    int status;
    waitpid(pids[i], &status, 0);
   
}
}

// internal commands : myexit function to exit the shell
void myexit() {
    exit(0);
}

// internal commands : mycd function to change the directory
void mycd(const vector<string>& args) {
if (args.size() < 2) {
cerr << "mycd: missing argument" << endl;
return;
}
if (chdir(args[1].c_str()) == -1) {
perror("chdir");
}
}

// internal commands : mypwd function to print the present working directory
void mypwd() {
    char* cwd = getcwd(nullptr, 0);
    cout << cwd << endl;
    free(cwd);
}

//main function begins
int main() {
while (true) {
printPrompt();
string command;
getline(cin, command);

if (cin.eof()) {
cout << endl;
break;
}
if (command.empty()) {
continue;
}    



//split the commands
    vector<vector<string>> commands;
    vector<string> args;
    string input_file, output_file;
    bool redirect_input = false, redirect_output = false;
    for (string arg : parse_command(command)) {
        if (arg == "<") {
            redirect_input = true;
        } else if (arg == ">") {
            redirect_output = true;
        } else if (redirect_input) {
            input_file = arg;
            redirect_input = false;
        } else if (redirect_output) {
            output_file = arg;
            redirect_output = false;
        } else if (arg == "|") {
            commands.push_back(args);
            args.clear();
        } else {
            args.push_back(arg);
        }
    }
    if (!args.empty()) {
        if (args[0] == "mycd") {
            mycd(args);
            continue;
        } else if (args[0] == "mypwd") {
            mypwd();
            continue;
        } else if (args[0] == "myexit") {
            exit(0);
        }
        commands.push_back(args);
    }

    //call execute function
    execute_pipeline(commands, input_file, output_file);
}
return 0;
}