#include <stdbool.h> 
#include <dirent.h>
#include <string.h>
#include "libft.h"
#include "ft_printf.h"
#include <stdlib.h>
#include <sys/stat.h>

typedef struct args {
	bool	l_flag;
	bool	a_flag;
	bool	r_flag;
	bool	R_flag;
	bool	t_flag;
}	t_args;

//need some kind of data structure for storing director information.

//t_args	parse_args(char *args) {
	

//}

int	read_read(char *path) {
	DIR *dirp;
	struct dirent *dp;
	struct stat	*sp = malloc(sizeof(struct stat));

	dirp = opendir(path);
	if (dirp == NULL)
		return (1);
	dp = readdir(dirp);
	while (dp != NULL) {
		stat(dp->d_name, sp);
		//ft_printf("FILE NAME %s\n", dp->d_name);
		//ft_printf("FILE SIZE %d\n", sp->st_blocks);
		if ((dp->d_name[0] == '.' && dp->d_name[1] == '\0') || (dp->d_name[1] == '.' && dp->d_name[2] == '\0')) {
			//this filters out . and .. we dont want to recurse over those.
			dp = readdir(dirp);
			continue ;
		}
		//ft_printf("sp->d_name[0] == . : %d\n", dp->d_name[0] == '.');
	        //if ((dp->d_name[0] == '.') && (dp->d_name[1] == '\0') || ((dp->d_name[1] == '.') && (dp->d_name[2] == '\0'))) {
		//	continue ;
		//}
		if (S_IFDIR & sp->st_mode) {
			//ft_printf("FILE is a dir %s\n", dp->d_name);
			//so here I want to have a linked list. kind of build a strucutre???? or just fucking printing it? how will I reverse it? Well u can't :D it makes sense to read in all the information in some kind of linked list and the do the priting I guess that is why in the subject they say for accounting for -R.
			//how do we doo this some 
			/*

some struct linked list that would be a dir
then we weould say if a then we would say het I am parent dir which is always the first object read in I think the . some then we know that all the files have this set to false ro something
when we print we check hey parent dir and all the other files that follow are regular files in that dir are just files.
. dir
file file file file file
fiel 
.dir
*/
			//kaut ka te vaig pielikt klat iepreiksejo path....
			char *tmp = ft_strjoin(path, "/");
			char *tmp2 = ft_strjoin(tmp, dp->d_name);
			ft_printf("going to %s\n", tmp2);
			read_read(tmp2);
			free(tmp);
			free(tmp2);
		}
		ft_printf("name %s\n", dp->d_name);
		dp = readdir(dirp);
	}
	closedir(dirp);
	return 0;
}

int	x(char **argv) {
	int index;

	index = 1;
	if (argv[index][0] == '-') {
		if (ft_strchr(argv[index], 'R')) {
			index = 2;
			ft_printf("user want to RECUREST\n");
			//here I would say we build a data strucure
			//so we just get all names stat and open dirs maybe same some info for printf
			while (argv[index]) {
				read_read(argv[index]);
				index++;
			}
		}
/*
		if (ft_strchr(argv[index], 't')) {
			ft_printf("user want to t\n");
			//here we sort by most recently modified time
		}
		if (ft_strchr(argv[index], 'r')) {
			ft_printf("user want to recurest\n");
			//here we just reverse some kind of order that ls uses
			//this goes last since se can just reverse the order of our data strucutre
		}
		if (ft_strchr(argv[index], 'a')) {
			ft_printf("user want to hiden files\n");
			//include hiden files in your output
		}
		if (ft_strchr(argv[index], 'l')) {
			ft_printf("user want a list\n");
			//here we will just us a different priting ofrmat
		}
*/
	}
	//
	return (0);
}

int	main(int argc, char **argv) {
	(void)argc;
	//if (argc == 1) {
		
	//}
	x(argv);
	return (0);
}
