#include <stdio.h>
#include <map>
#include <string>
#include <limits.h>
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
};

