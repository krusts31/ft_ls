#include <stdbool.h> 
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <errno.h>
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

char	*build_string(struct stat *sp) {
	char	*tmp;

	tmp = ft_calloc(sizeof(char), 12);
	ft_memset(tmp, '-', 11);
	if (tmp == NULL)
		return NULL;
	if (S_IFDIR & sp->st_mode)
		tmp[0] = 'd';
	if (S_IRUSR & sp->st_mode)
		tmp[1] = 'r';
	if (S_IWUSR & sp->st_mode)
		tmp[2] = 'w';
	if (S_IXUSR & sp->st_mode) 
		tmp[3] = 'x';
	if (S_IROTH & sp->st_mode)
		tmp[4] = 'r';
	if (S_IWOTH & sp->st_mode)
		tmp[5] = 'w';
	if (S_IXOTH & sp->st_mode)
		tmp[6] = 'x';
	if (S_IRGRP & sp->st_mode)
		tmp[7] = 'r';
	if (S_IWGRP & sp->st_mode)
		tmp[8] = 'w';
	if (S_IXGRP & sp->st_mode)
		tmp[9] = 'x';
	tmp[10] = '@';
	return tmp;
}

void	print_l(struct dirent *dp, struct stat *sp, struct stat *lp) {
	(void) dp;
	struct group *grp;
	struct passwd *pswd;
	char	**bits;
	char	*time_bit;
	//ssize_t bufsize;
	char	*l;

	char	 *premissinons = build_string(sp);	
	if (premissinons == NULL)
		return ;
	if (S_IFLNK & lp->st_mode) {
		ft_printf("\nlink\n");
	}
	//bufsize = sp->st_size + 1;
	//if (sp->st_size == 0) {
//		bufsize = PATH_MAX;
//	}

	grp = getgrgid(sp->st_gid);
	pswd = getpwuid(sp->st_uid);
	l = ctime(&sp->st_mtime);
//%jd, (intmax_t) sp->st_size, 
	bits = ft_split(l, ' ');
	time_bit = ft_calloc(sizeof(char), 6);
	if (time_bit == NULL) {
		return ;
	}
	ft_memmove(time_bit, bits[3], 5);
	ft_printf("%s %s %s %d %s %s %s %s\n", premissinons, pswd->pw_name, grp->gr_name, sp->st_size, bits[2], bits[1], time_bit,  dp->d_name);
}

int	read_read(char *path, t_args x) {
	DIR *dirp;
	struct stat	*sp;
	struct stat	*lp;
	struct dirent *dp;
	bool print_new_line	= false;

	dirp = opendir(path);
	dp =  ft_calloc(sizeof(struct dirent), 1);
	if (dirp == NULL)
		return (1);
	dp = readdir(dirp);

	//in stadart mode it prints
	//first all files.
	//the dirs

	
	while (dp != NULL) {
		//ft_printf("FILE NAME %s\n", dp->d_name);
		//ft_printf("FILE SIZE %d\n", sp->st_blocks);
		if ((dp->d_name[0] == '.' && dp->d_name[1] == '\0') || (dp->d_name[1] == '.' && dp->d_name[2] == '\0')) {
			//this filters out . and .. we dont want to recurse over those.
			if (print_new_line == true) {
				ft_printf("\n");
				print_new_line = false;
			}
			dp = readdir(dirp);
			continue ;
		}
		sp = ft_calloc(sizeof(struct stat), 1);
		lp = ft_calloc(sizeof(struct stat), 1);
		//aizmirsu domu sim
		if (!(path[0] == '.' && path[1] == '\0')) {
			char *tmp4 = ft_strjoin(path, "/");
			char *tmp3 = ft_strjoin(tmp4, dp->d_name);
			if (stat(tmp3, sp)) {
				perror(strerror(errno));
				printf("STAT ERROR!!! %s\n", dp->d_name);
			}
			if (lstat(tmp3, lp) < 0) {
				perror(strerror(errno));
				printf("LSTAT ERROR!!! %s\n", dp->d_name);
			}
			if (S_ISLNK(lp->st_mode) == 1) {
			}
			free(tmp4);
			free(tmp3);
		//aizmirsu domu sim laikam ja local dir .?
		} else {
			if (stat(dp->d_name, sp)) {
				perror(strerror(errno));
				printf("STAT ERROR!!! %s\n", dp->d_name);
			}
			if (lstat(dp->d_name, lp) < 0) {
				perror(strerror(errno));
				printf("LSTAT ERROR!!! %s\n", dp->d_name);
			}
			if (S_ISLNK(lp->st_mode) == 1) {
				char	buf[1024];

				ssize_t	x = readlink(dp->d_name, buf, (sizeof(buf) - 1));
				buf[x] = '\0';
				printf("BUF: %s\n", buf);
				//buff just has the sstuff that the link points to
			}
		}
		if (S_IFDIR & sp->st_mode && x.R_flag) {
			if (!x.a_flag && dp->d_name[0] == '.') {
				free(sp);
				dp = readdir(dirp);
				continue ;
			}
			char *tmp = ft_strjoin(path, "/");
			char *tmp2 = ft_strjoin(tmp, dp->d_name);
			//ft_printf("going to %s\n", tmp2);
			ft_printf("\n%s:\n",tmp2);
			print_new_line = false;
			read_read(tmp2, x);
			free(tmp);
			free(tmp2);
			//push to stack?
		}
		//
		if (!(!x.a_flag && dp->d_name[0] == '.')) {
			if (x.l_flag) {
				//in case we have a link
				lp = ft_calloc(sizeof(struct stat), 1);
				if (lp == NULL)
					return 1;
				print_l(dp, sp, lp);
			} else {
				ft_printf("%s\t", dp->d_name);
				print_new_line = true;
			}
		}
		free(sp);
		dp = readdir(dirp);
	}
	closedir(dirp);
	if (print_new_line == true || dp == NULL)
		ft_printf("\n");
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
