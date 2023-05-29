
# Compiler and flags
CC = g++
FLAGS = -std=c++11

all: mytree.x mytime.x mymtimes.x mytimeout.x mytoolkit.x

mytree.x: mytree.o
	$(CC) $(FLAGS) -o mytree.x mytree.o

mytime.x: mytime.o
	$(CC) $(FLAGS) -o mytime.x mytime.o

mymtimes.x: mymtimes.o
	$(CC) $(FLAGS) -o mymtimes.x mymtimes.o

mytimeout.x: mytimeout.o
	$(CC) $(FLAGS) -o mytimeout.x mytimeout.o

mytoolkit.x: mytoolkit.o
	$(CC) $(FLAGS) -o mytoolkit.x mytoolkit.o


mytree.o: mytree.cpp
	$(CC) $(FLAGS) -c mytree.cpp

mytime.o: mytime.cpp
	$(CC) $(FLAGS) -c mytime.cpp

mymtimes.o: mymtimes.cpp
	$(CC) $(FLAGS) -c mymtimes.cpp

mytimeout.o: mytimeout.cpp
	$(CC) $(FLAGS) -c mytimeout.cpp

mytoolkit.o: mytoolkit.cpp
	$(CC) $(FLAGS) -c mytoolkit.cpp

clean:
	rm -f *.x *.o

