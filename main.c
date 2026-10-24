#include "libft.h"
#include "ft_printf.h"

void	x() {
	dirp = opendir(".");
	if (dirp == NULL)
		   return (ERROR);
	len = strlen(name);
	while ((dp = readdir(dirp)) != NULL) {
		   if (dp->d_namlen == len && strcmp(dp->d_name, name) == 0) {
				   (void)closedir(dirp);
				   return (FOUND);
		   }
	}
	(void)closedir(dirp);
	return (NOT_FOUND);
}

int	main(int argc, char **argv) {

	
	ft_printf("Hello World");
	return (0);
}
