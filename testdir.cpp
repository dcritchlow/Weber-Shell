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
using namespace std;

int main(int argc, char **argv) {
	struct stat statbuf;

	int rc = stat(argv[1], &statbuf);
	if (rc) {
		perror(argv[1]);
		return 1;
	}
	// print a message if argv[1] is a directory
	if (S_ISDIR(statbuf.st_mode)) {
		cout << "Dude, " << argv[1] << " is a Directory" << endl;
	} else {
		cout << "Dude, " << argv[1] << " is NOT a Directory" << endl;

	}	
	return 0;
}
