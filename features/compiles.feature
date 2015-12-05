Feature: Program must compile with no errors, print a prompt and exit
	Scenario: wsh.cpp and wsh.h must be found
		When I run `getfiles` 
		And a file named "../bin/wsh.cpp" should exist
		And a file named "../bin/wsh.h" should exist
		And I run `g++ -ggdb -Wall -Wno-write-strings -o ../bin/wsh ../bin/wsh.cpp` 
		And the exit status should be 0
 		And the stderr should not contain anything
		And I run `../bin/wsh` interactively
		And I type "exit"
		Then the output should contain PWD
		And the exit status should be 0
 		And the stderr should not contain anything
		Then 20 points are awarded
