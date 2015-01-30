#include "wsh.h"
#include <iostream>
#include <string>
#include <cstring>
#include <map>
#include <unistd.h>
#include <stdlib.h>
using namespace std;



wsh::wsh()
{
	// recognized commands
	char*	commands[] = { "-place holder-", "exit", "copy" };


	// builds command name hash map
	for (unsigned i = 1; i < sizeof(commands)/sizeof(char*); i++)
		command_table[commands[i]] = i;

	system("clear");
	// LAB 1: initialize the cwd member variable here.
    getcwd(cwd, sizeof(cwd));

}



int wsh::next_command()
{
	// LAB 1: place assignment code here.
	// Note that the program will not run until this part is completed.
	// define a command buffer and a place for the current working directory string

    static char cmd[PATH_MAX];
    char *token;
    argc = 0;

    cout << cwd << "=> ";
    cin.getline(cmd, PATH_MAX);
    token = strtok(cmd, " \t");
    if (token == NULL)
        return NONE;

    while(token != NULL)
    {
        argv[argc++] = token;
        token = strtok(NULL, " \t");

    }

    if (strcmp(argv[0], "copy") == 0)
        // returns -1 if str1 is bigger 0 if equal +1 if str2 is bigger
        return COPY;
    if (strcmp(argv[0], "exit") == 0)
        // returns -1 if str1 is bigger 0 if equal +1 if str2 is bigger
        return EXIT;
    return 0;
}

void wsh::copy()
{
//    printf("%s\n", "I am in copy");

    char *fileNameAfterRightSlash = strrchr(argv[1], '/');
    int copyToCurrentDirectory = strcmp(argv[2], ".");
    if (copyToCurrentDirectory != 0 && strcmp(argv[1], argv[2]) != 0)
    {
        cout << "copy " << argv[1] << " " << argv[2] << endl;
    }
    else if (copyToCurrentDirectory == 0)
    {
        if (fileNameAfterRightSlash == NULL)
        {
            cout << "copy " << argv[1] << " " << cwd << "/" << argv[1] << endl;
        }
        else
        {
            cout << "copy " << fileNameAfterRightSlash << " " << cwd << fileNameAfterRightSlash << endl;
        }
    }
    else
    {
        cout << "Invalid argument!";
    }
//    printf("%s\n", p);
}

void wsh::interpret()		// loops executing commands from the command line.
{
	bool	running = true;

	while (running)
	{
		int	c = next_command();

		switch(c)
		{
			case EXIT :
				running = false;
				break;

			case COPY :
				copy();
				break;

			case NONE :		// no command entered
				break;

			default:		// tests command line parsing
				for (int i = 0; i < argc; i++)
					cout << argv[i] << endl;
			      break;
		}
	}
}

int main()		// execution startup.
{
	wsh	cli;

	cli.interpret();

	return 0;
}

