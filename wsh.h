#include <stdio.h>
#include <map>
#include <string>
#include <limits.h>
using namespace std;



enum { NONE = -1, NOT_FOUND, EXIT, COPY };



class wsh
{
	private:
		map<string, int, less<string> >	command_table;
		char				cwd[PATH_MAX];
		int				argc;
		char*				argv[FILENAME_MAX];

	public:
			wsh();

		int	next_command();
		void	interpret();

		void	copy();
};

