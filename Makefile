all: wsh peek
wsh: wsh.cpp wsh.h
	g++ -ggdb -Wall -Wno-write-strings -o wsh wsh.cpp
peek: peek.cpp
	g++ -ggdb -Wall -Wno-write-strings -o peek peek.cpp
