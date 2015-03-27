#include "wsh.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <map>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
//#include <InstallerPlugins/InstallerPlugins.h>
//#include <InstallerPlugins/InstallerPlugins.h>

using namespace std;



// used to convert numeric file modes to string representation

char*	fmodes[] = { "---", "--x", "-w-", "-wx", "r--", "r-x", "rw-", "rwx" };


// -----------------------------------------------------------------------------------



wsh::wsh()
{
	char*	commands[] = { "-place holder-", "exit", "copy", "cd", "list", 
		"view", "del", "ren", "mkdir", "rmdir" };

	for (unsigned i = 1; i < sizeof(commands)/sizeof(char*); i++)
		command_table[commands[i]] = i;

	system("clear");
	getcwd(cwd, PATH_MAX);
}


int wsh::next_command()
{
	// LAB 1: place assignment code here.
	// Note that the program will not run until this part is completed.
	// define a command buffer and a place for the current working directory string
	static char cmd[PATH_MAX];
	char *token;
	// display current working directory
	cout << cwd;
	cout << "=>";
	// get a command and arguments from the user, count and parse into argv[]
	cin.getline(cmd, sizeof(cmd));
	// reset argc
	argc = 0;
	// while tokens available, assign them to argv using argc as index
	token = strtok(cmd, " \t");
	while (token != NULL) {
		argv[argc++] = token;
		token = strtok(NULL, " \t"); 
	}
	// if there were no tokens, return NONE else return token from ENUM
	if (argv[0] == NULL)
		return NONE;
	else
		return command_table[argv[0]];
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

			case CD :
				cd();
				break;

			case LIST :
				list();
				cout << endl;
				break;
			case VIEW :
				view();
				break;

			case DEL :
				del();
				break;

			case REN :
				ren();
				break;

			case MKDIR :
				makedir();
				break;

			case RMDIR :
				removedir();
				break;

			case NONE :
				break;

			default:
				for (int i = 0; i < argc && argv[i] != NULL; i++)
					cout << argv[i] << endl;
			      break;
		}
	}
}
void wsh::copy()
{
    if (argc != 3){
		cout << "Invalid argument count" << endl;
        return;
    }
    if (argc == 3){
        if (strcmp(argv[1], argv[2]) == 0) {
            cout << argv[1] << " " << argv[2] << endl;
            cout << "Both files have the same name" << endl;
            return;
        }
        copy(argv[1], argv[2]);
    }
}

bool wsh::copy(char* src, char* dest)
{
    struct stat statbuf;
//	DIR *dir;
//	struct dirent *entry;

    int rc = stat(argv[1], &statbuf);
    if (rc) {
        perror(argv[1]);
        return 1;
    }

    if (S_ISDIR(statbuf.st_mode)) {
		char str[100];
		sprintf(str, "cp -r %s %s",src, dest);
		system(str);
    }
    else {
        ifstream source(src, ios::binary);
        bool isOpen = source.is_open();
        if (!isOpen){
            perror("Error while opening file");
            return false;
        }
        ofstream target(dest, ios::binary);
        target << source.rdbuf();

        source.close();
        target.close();
        return true;
    }

    return 0;

}

void wsh::cd(){

}

void wsh::cd(char* dir) {
	int rc;
	rc = chdir(argv[1]);
	if (rc){
		perror(argv[1]);
	}

	return;
}

void wsh::list() {
    list(argc, argv);
}

int wsh::list(int argc, char **argv) {
	struct stat statbuf;
//	struct tm *t;
//	int rc;
//	char buffer[PATH_MAX];
	DIR *dir;
	struct dirent *ent;

	if (argc != 2) {
		if ((dir = opendir (cwd)) != NULL) {

			while( (ent = readdir(dir)) != NULL ) {
				if( strcmp(ent->d_name, ".") == 0 ||  strcmp(ent->d_name, "..") == 0)
					continue;

				lstat(ent->d_name, &statbuf);

				printf("%s", ent->d_name);

				if( S_ISDIR(statbuf.st_mode) )
					puts("/");
				else if ( statbuf.st_mode & S_IXUSR )
					puts("*");
				else
					puts("");
			}
			closedir (dir);
		}
		else {
			/* could not open directory */
			perror (cwd);
			return EXIT_FAILURE;
		}
	}

	if (argc == 2) {
		if (strcmp(argv[1], "-l") != 0) {
//			puts("not -l");

			if ((dir = opendir (argv[1])) != NULL) {
//				puts("opened dir");

				while( (ent = readdir(dir)) != NULL ) {
//					puts("in while");
					if( strcmp(ent->d_name, ".") == 0 ||  strcmp(ent->d_name, "..") == 0)
						continue;

//					puts("after . check");
					stat(ent->d_name, &statbuf);

					printf("%s", ent->d_name);

					if( S_ISDIR(statbuf.st_mode) )
						puts("Directory");
					else if ( S_ISREG(statbuf.st_mode) )
						puts("File");
					else if ( S_ISCHR(statbuf.st_mode) )
						puts("Character Device");
					else if ( S_ISBLK(statbuf.st_mode) )
						puts("Block Device");
					else if ( S_ISFIFO(statbuf.st_mode) )
						puts("Fifo");
					else if ( S_ISLNK(statbuf.st_mode) )
						puts("Link");
					else
						puts("Socket");
				}
				closedir (dir);
			}
			else {
				/* could not open directory */
				perror (cwd);
				return EXIT_FAILURE;
			}
		}

		if (strcmp(argv[1], "-l") == 0) {
			puts("yes its  -l");
			if ((dir = opendir(cwd)) != NULL) {
				while ((ent = readdir(dir))) {
					if (ent->d_type & DT_REG)
						cout << ent->d_name << endl;
					else if (ent->d_type & DT_DIR) if (!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, ".."))
						cout << ent->d_name << endl;
					else
						cout << ent->d_name << "/" << endl;
					else if (ent->d_type & DT_LNK)
						cout << ent->d_name << " is a symbolic link" << endl;
					else if (strcmp(ent->d_name, ".") || strcmp(ent->d_name, ".."))
						cout << ent->d_name << endl;
					else
						cout << ent->d_name << " is something other than a file or directory" << endl;
				}
				closedir(dir);
			} else {
				/* could not open directory */
				perror(argv[1]);
				return EXIT_FAILURE;
			}
		}
	}
	return EXIT_SUCCESS;
}


void wsh::view() {
	struct stat statbuf;

    if (argc != 2){
        cout << "Invalid argument count" << endl;
        return;
    }
    if (argc == 2){
		int rc = stat(argv[1], &statbuf);
		if (rc) {
			perror(argv[1]);
			return;
		}
		ifstream source(argv[1], ios::binary);
		bool isOpen = source.is_open();
		if (!isOpen){
			perror("Error while opening file");
			return;
		}

		cout << source.rdbuf();

		source.close();
        return;
    }
}

void wsh::del() {
	struct stat statbuf;
	int rc;

    if (argc != 2){
		cout << "Invalid argument count" << endl;
        return;
    }
    if (argc == 2){
		rc = stat(argv[1], &statbuf);
		if (rc) {
			perror(argv[1]);
			return;
		}
		unlink(argv[1]);
        return;
    }
}

void wsh::ren() {
	struct stat statbuf;

    if (argc != 3){
		cout << "Invalid argument count" << endl;
        return;
    }
    if (argc == 3){
		int rc = stat(argv[1], &statbuf);
		if (rc) {
			perror(argv[1]);
			return;
		}

		ifstream source(argv[1], ios::binary);
		bool isOpen = source.is_open();
		if (!isOpen){
			perror("Error while opening file");
			return;
		}
		ofstream target(argv[2], ios::binary);
		target << source.rdbuf();

		source.close();
		target.close();
		unlink(argv[1]);

        return;
    }
}

void wsh::makedir() {
    int rc;
    if (argc != 2){
        cout << "Invalid argument count" << endl;
        return;
    }
    if (argc == 2){
        rc = mkdir(argv[1], S_IRWXU|S_IRWXG|S_IRWXO);
        if (rc){
            perror(argv[1]);
        }
        return;
    }
}
void wsh::removedir()
{
	// verify 2 or 3 arguments only
	if (argc != 2 && argc != 3) {
		cout << "Invalid arguments to rmdir";
		return;
	}

	// 3 args means rmdir -s DIR: verify the -s and recursively delete everything
	if (argc == 3) {
		if (strcmp(argv[1], "-s")) {
			cout << "Invalid arguments to rmdir";
			return;
		}
		// call a recursive algorithm to delete everything inside a dir plus the dir itself
		if (!removedir(argv[2])) {
			cout << "Falure deleting " << argv[2] << endl;
		}
		return;
	}
	
	// -s not specified, simply remove the directory
	if (rmdir(argv[1])) {
		perror(argv[1]);
	}
}

bool wsh::removedir(char *dirName)
{
	DIR *dir;
	struct dirent *dirEntry;
	bool success;
	int rc;

	cout << "processing subdirectory " << dirName << endl;
	if (!(dir = opendir(dirName))) {
		perror(dirName);
		return false;
	}
	// chdir into directory to process its entries
	cout << "chdir into " << dirName << endl;
	chdir(dirName);
	while ((dirEntry = readdir(dir))) {
		// if a directory found, recurse to delete everything in it
		if (dirEntry->d_type & DT_DIR) {
			// skip "." and ".."
			if (!strcmp(dirEntry->d_name, "."))
				continue;
			if (!strcmp(dirEntry->d_name, ".."))
				continue;
			cout << "found subdirectory " << dirEntry->d_name << endl;
			success = removedir(dirEntry->d_name);
			if (!success) {
				return false;
			}
		} else {
			// otherwise delete the entry
			cout << "deleting " << dirEntry->d_name << endl;
			// success = true;
			rc = remove(dirEntry->d_name);
			if (rc) {
				perror(dirEntry->d_name);
				return false;
			} 
		}
	}
	// directory is now empty -- chdir out of it and delete it
	cout << "chdir out of " << dirName << endl;
	chdir("..");
	cout << "deleting " << dirName << endl;
	// success = true;
	rc = rmdir(dirName);
	if (rc) {
		perror(dirName);
		return false;
	}
	return true;
}

int main()		// execution startup.
{
	wsh	cli;

	cli.interpret();

	return 0;
}

