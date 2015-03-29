#include <stdio.h>
#include <map>
#include <string>
#include <limits.h>
#include <dirent.h>

using namespace std;



enum { NONE = -1, NOT_FOUND, EXIT, COPY, CD, LIST, VIEW, DEL, REN, MKDIR, RMDIR };



class wsh
{
	private:
		map<string, int, less<string> >	command_table;
		char				cwd[PATH_MAX];
		int				argc;
		char*				argv[FILENAME_MAX];
		bool	removedir(char *dir);
        bool    copy(char *src, char *dest);
		void	cd(char *dir);
		int		list(int argc, char **argv);

	public:
			wsh();

		int	next_command();
		void	interpret();

		void	cd();
		void	list();
		void	copy();
		void	view();
		void	del();
		void	ren();
		void	makedir();
		void	removedir();

	bool isDotDirectory(dirent * ent);

	static void printLongList(struct stat &statbuf, tm *t, dirent *ent);

	void printShortList(struct stat &statbuf, dirent *ent);
	void printShortListDir(struct stat &statbuf, dirent *ent);

	int isADirectory(struct stat &statbuf, char *const ent);

	void printDirectory(struct stat &statbuf, DIR *dir, dirent *ent, char *currentDir);

//	void printDirectoryLong(tm *t, struct stat &statbuf, DIR *&dir, dirent *&ent, char *currentDir);
	void printDirectoryLong(tm *t, struct stat &statbuf, DIR *dir, dirent *ent, char *currentDir);

	void printDirectoryWithSubDirectory(struct stat &statbuf, DIR *dir, dirent *ent, char *currentDir);
	void printDirectoryWithSubLong(tm *t, struct stat &statbuf, DIR *dir, dirent *ent, char *currentDir);
};

