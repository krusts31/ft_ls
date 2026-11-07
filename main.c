#include <stdbool.h> 
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <uuid/uuid.h>
#include <time.h>
#include <dirent.h>
#include <string.h>
#include "libft.h"
#include "ft_printf.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

//need to handle -lar
//and -l -a -r
//at the same time
typedef struct args {
	bool	l_flag;
	bool	a_flag;
	bool	r_flag;
	bool	R_flag;
	bool	t_flag;
}	t_args;

typedef struct s_dir {
	char	*path;
	struct	s_dir	*next;
}	t_dir;

//need some kind of data structure for storing director information.

//t_args	parse_args(char *args) {
	

//}

//maybe the logic is first print all the files
//and if you ecounter a directory push taht on a stack and then just loop over the stack?

//its more that current dir is respected its not about how deep you ar ein recursions its more like
//so first we need to finsh looping over all files and then we can open a dir go inside it and print it 
//jjjkk

char	*get_right_time(char *test) {
	char **bits;
	char *tmp;
	char	*min;

	min = ft_calloc(sizeof(char),6);
	bits = ft_split(test, ' ');
	tmp = ft_strjoin(bits[2], bits[1]);
	ft_memmove(min, bits[3], 5);
	return ft_strjoin(tmp, min);
}

void	print_l(struct dirent *dp, struct stat *sp) {
	(void) dp;
	struct group *grp;
	struct passwd *pswd;
	char	*l;

	grp = getgrgid(sp->st_gid);
	pswd = getpwuid(sp->st_uid);
	l = ctime(&sp->st_mtime);
	char *curretct_time = get_right_time(l);
//%jd, (intmax_t) sp->st_size, 
	printf("time <%s> %s %s %s\n", curretct_time, pswd->pw_name, grp->gr_name, dp->d_name);

	printf("%s\n", l);
	//free(time);
	//ft_printf("Last status change:       %s", ctime(sb->st_ctime));
	//ft_printf("Last file access:         %s", ctime(sb->st_atime));
}

int	read_read(char *path, t_args x) {
	DIR *dirp;
	struct dirent *dp;
	struct stat	*sp = malloc(sizeof(struct stat));

	dirp = opendir(path);
	if (dirp == NULL)
		return (1);
	dp = readdir(dirp);

	//in stadart mode it prints
	//first all files.
	//the dirs

	
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
		if (S_IFDIR & sp->st_mode && x.R_flag) {
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
			//
			//kaut ka te vaig pielikt klat iepreiksejo path....
			if (!x.a_flag && dp->d_name[0] == '.') {
				dp = readdir(dirp);
				continue ;
			}
			char *tmp = ft_strjoin(path, "/");
			char *tmp2 = ft_strjoin(tmp, dp->d_name);
			//ft_printf("going to %s\n", tmp2);
			read_read(tmp2, x);
			free(tmp);
			free(tmp2);
			//push to stack?
		}
		//
		if (!x.a_flag && dp->d_name[0] == '.') {
			dp = readdir(dirp);
			continue ;
		} else {
			if (x.l_flag) {
				print_l(dp, sp);
			} else {
				ft_printf("name %s\n", dp->d_name);
			}
		}
		dp = readdir(dirp);
	}
	closedir(dirp);
	return 0;
}

int	x(char **argv) {
	int index;
	bool	path = true;
	t_args x;

	index = 1;
	while (argv[index]) {
		if (argv[index][0] != '-') {
			path = false;
			break ;
		}
		index++;
	}
	index = 1;
	while (argv[index]) {
		if (argv[index][0] == '-') {
			if (ft_strchr(argv[index], 't')) {
				x.t_flag = true;
				//here we sort by most recently modified time
			}
			if (ft_strchr(argv[index], 'r')) {
				x.r_flag = true;
				//here we just reverse some kind of order that ls uses
				//this goes last since se can just reverse the order of our data strucutre
			}
			if (ft_strchr(argv[index], 'a')) {
				x.a_flag = true;
				//include hiden files in your output
			}
			if (ft_strchr(argv[index], 'l')) {
				x.l_flag = true;
				//here we will just us a different priting ofrmat
			}
			if (ft_strchr(argv[index], 'R')) {
				index = 2;
				x.R_flag = true;
				//here I would say we build a data strucure
				//so we just get all names stat and open dirs maybe same some info for printf
				while (argv[index]) {
					read_read(argv[index], x);
					index++;
				}
			}
		} else {
			read_read(argv[index], x);
		}
		index++;
	}
	if (path) {
		read_read(".", x);
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
