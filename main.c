#include <stdbool.h> 
#include <dirent.h>
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
		ft_printf("%s\n", dp->d_name);
		ft_printf("FILE SIZE %d\n", sp->st_blocks);
		if (S_IFDIR & sp->st_mode) {
			ft_printf("FILE is a dir %s\n", dp->d_name);
			//so here I want to have a linked list. kind of build a strucutre???? or just fucking printing it? how will I reverse it? Well u can't :D it makes sense to read in all the information in some kind of linked list and the do the priting I guess that is why in the subject they say for accounting for -R.
			return read_read(dp->d_name);
		}
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
