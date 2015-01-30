wsh: wsh.cpp wsh.h
	g++ -ggdb -Wall -Wno-write-strings -o wsh wsh.cpp
clean:
	rm wsh
