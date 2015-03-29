#include "wsh.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <map>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <time.h>
#include <ftw.h>
#include <inttypes.h>

using namespace std;



// used to convert numeric file modes to string representation

char*	fmodes[] = { "---", "--x", "-w-", "-wx", "r--", "r-x", "rw-", "rwx" };


// -----------------------------------------------------------------------------------



//static int display_info(const char *fpath, const struct stat *sb, int tflag);

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
	cd(argv[1]);
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
//	struct FTW ftwbuf;
	struct tm *t = NULL;
//	int flags = 0;
//	char buffer[PATH_MAX];
	DIR *dir = NULL; 
	struct dirent *ent = NULL;

	if(strcmp(argv[0], "list") == 0 && argc ==1){

		printDirectory(statbuf, dir, ent, cwd);

	}

	if (strcmp(argv[0], "list") == 0 && strcmp(argv[1], "-l") == 0 && argc == 2) {

		printDirectoryLong(t, statbuf, dir, ent, cwd);

	}
	if (strcmp(argv[0], "list") == 0 && strcmp(argv[1], "-ls") == 0 && argc == 2) {

		printDirectoryWithSubLong(t, statbuf, dir, ent, cwd);

	}

	if (strcmp(argv[0], "list") == 0 && strcmp(argv[1], "-s") == 0 && argc == 2) {

		printDirectoryWithSubDirectory(statbuf, dir, ent, cwd);

	}
	return EXIT_SUCCESS;
}

void wsh::printDirectoryWithSubDirectory(struct stat &statbuf, DIR *dir, dirent *ent, char *currentDir) {
	if ((dir = opendir (currentDir)) != NULL) {

		while( (ent = readdir(dir)) != NULL ) {
			if(isDotDirectory(ent))
				continue;

			lstat(ent->d_name, &statbuf);

			if(isADirectory(statbuf, ent->d_name) == 1) {

				char *currDir = strcat(getcwd(currentDir, PATH_MAX), "/");
				currDir = strcat(currDir, ent->d_name);

				printf("%s:\n",ent->d_name);

				printDirectory(statbuf, dir, ent, currDir);
			}
			else{
				printShortList(statbuf, ent);
			}



		}
		closedir(dir);
	}
	else {
		perror (cwd);
	}
}

void wsh::printDirectoryWithSubLong(tm *t, struct stat &statbuf, DIR *dir, dirent *ent, char *currentDir) {
	if ((dir = opendir (currentDir)) != NULL) {

		while( (ent = readdir(dir)) != NULL ) {
			if(isDotDirectory(ent))
				continue;

			lstat(ent->d_name, &statbuf);
			t = localtime(&statbuf.st_mtime);

			if(isADirectory(statbuf, ent->d_name) == 1) {

				char *currDir = strcat(getcwd(currentDir, PATH_MAX), "/");
				currDir = strcat(currDir, ent->d_name);

				printf("%s:\n",ent->d_name);

				printDirectoryLong(t, statbuf, dir, ent, currDir);

			}
			else{
				printLongList(statbuf, t, ent);

			}



		}
		closedir(dir);
	}
	else {
		perror (cwd);
	}
}

void wsh::printDirectoryLong(tm *t, struct stat &statbuf, DIR *dir, dirent *ent, char *currentDir) {
	if ((dir = opendir (currentDir)) != NULL) {

		while( (ent = readdir(dir)) != NULL ) {
			if( isDotDirectory(ent) )
				continue;

			lstat(ent->d_name, &statbuf);
			t = localtime(&statbuf.st_mtime);

			printLongList(statbuf, t, ent);

		}
		closedir(dir);
	}
	else {
		perror (currentDir);
	}
}

int wsh::isADirectory(struct stat &statbuf, char *const ent) {

	if( S_ISDIR(statbuf.st_mode) == 0 ){

		return EXIT_SUCCESS;

	}

	return EXIT_FAILURE;
}

char * wsh::printDirectory(struct stat &statbuf, DIR *dir, dirent *ent, char *currentDir) {
	if ((dir = opendir (currentDir)) != NULL) {

		while( (ent = readdir(dir)) != NULL ) {
			if(isDotDirectory(ent))
				continue;

			lstat(ent->d_name, &statbuf);

			printShortList(statbuf, ent);

		}
		closedir(dir);
	}
	else {
		/* could not open directory */
		perror (currentDir);
	}
	return NULL;
}



void wsh::printShortList(struct stat &statbuf, dirent *ent) {
	printf("%s%s\n"
	,ent->d_name
	,(ent->d_type & DT_DIR) ? "/" :
	(statbuf.st_mode & S_IXUSR) ? "*" :
	"");
}

void wsh::printShortListDir(struct stat &statbuf, dirent *ent) {
	if( S_ISDIR(statbuf.st_mode) == 0 ){
		puts("");
	}
	else{
		printf("%s%s\n", ent->d_name, (ent->d_type & DT_DIR) ? ":" :
				(statbuf.st_mode & S_IXUSR) ? "*" :
						"");
	}
}

void wsh::printLongList(struct stat &statbuf, tm *t, dirent *ent) {
	printf("%s%s%s%s%s%s%s%s%s%s %7jd %02d/%02d/%04d %02d:%02d:%02d %s\n"
	,(S_ISDIR(statbuf.st_mode)) ? "d" : "-"
	,(statbuf.st_mode & S_IRUSR) ? "r" : "-"
	,(statbuf.st_mode & S_IWUSR) ? "w" : "-"
	,(statbuf.st_mode & S_IXUSR) ? "x" : "-"
	,(statbuf.st_mode & S_IRGRP) ? "r" : "-"
	,(statbuf.st_mode & S_IWGRP) ? "w" : "-"
	,(statbuf.st_mode & S_IXGRP) ? "x" : "-"
	,(statbuf.st_mode & S_IROTH) ? "r" : "-"
	,(statbuf.st_mode & S_IWOTH) ? "w" : "-"
	,(statbuf.st_mode & S_IXOTH) ? "x" : "-"
	,(intmax_t) statbuf.st_size
	,t->tm_mon+1
	,t->tm_mday
	,t->tm_year+1900
	,t->tm_hour
	,t->tm_min
	,t->tm_sec
	,ent->d_name);
}

bool wsh::isDotDirectory(dirent *ent) {
	return strcmp(ent->d_name, ".") == 0 ||  strcmp(ent->d_name, "..") == 0;
}

//int display_info(const char *fpath, const struct stat *sb, int type)
//{
//	printf("%s", fpath);
//	if( S_ISDIR(sb->st_mode) )
//		puts("/");
//	else if ( sb->st_mode & S_IXUSR )
//		puts("*");
//	else
//		puts("");
//	return 0;
//}

//static int display_info(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf)
//{
//	printf("%7jd %s%s\n",
//
//			(intmax_t) sb->st_size,
//			fpath + ftwbuf->base, (tflag == FTW_D) ? "/" : "");
//	return 0;           /* To tell nftw() to continue */
//}


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

