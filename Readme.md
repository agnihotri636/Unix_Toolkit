Simple Unix systems toolkit for process and filesystem management, whose behavior is similar to both Unix shells (such as bash) and Unix systems forensic toolkits (such as Sleuth Kit).

Supports a number of commands that you need to implement: "myexit", "mycd", "mypwd".

Supports all existing unix commands, I/O redirections and pipes.

//Makefile
Compile all programs at once using command ‘make’

This will generate all .x files including mytoolkit.x
run mytoolkit.x to start shell

to run existing programs as mytree.x
$ mytree.x 

to run internal prgrams such as mycd
$ mycd folder_path/

to run IO and pipes please give spaces before and after > < |
$ ls > file.txt

Once all outputs generated, to clean the environment run ‘make clean’. This will delete all .o and .x files.