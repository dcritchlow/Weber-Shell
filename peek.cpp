#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <time.h>
using namespace std;

int main(int argc, char **argv) {
	struct stat statbuf;
	struct tm *t;
	int rc;
	char buffer[PATH_MAX];

	// issue a message if a file object was not specified on the commandline
	if (argc != 2) {
		cout << "Usage: " << argv[0] << " filename" << endl;
		return 1;
	}

	// get the inode information on the file object named in argv[1]
	rc = stat(argv[1], &statbuf);
	if (rc != 0) {
		perror(argv[1]);
		return 1;
	}

	// report on the type of the object
	if (S_ISREG(statbuf.st_mode)) 
		cout << argv[1] << " is a regular file" << endl;
	else if (S_ISDIR(statbuf.st_mode)) 
		cout << argv[1] << " is a directory" << endl;
	else if (S_ISLNK(statbuf.st_mode)) 
		cout << argv[1] << " is a symbolic link" << endl;
	else
		cout << argv[1] << " is something other than a file or directory" << endl;
	
	// report on the size of the object
	sprintf(buffer, "%s is %d bytes in size", argv[1], (int)statbuf.st_size);
	cout << buffer << endl;

	// report the date when the object was last modified
	t = localtime(&statbuf.st_mtime);
	sprintf(buffer, "%s was last modified on %02d/%02d/%04d", 
		argv[1], 
		t->tm_mon+1,
		t->tm_mday,
		t->tm_year+1900);
	cout << buffer << endl;

	return 0;
}
