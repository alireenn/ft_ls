#ifndef FT_LS
#define FT_LS
// ----------------------
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/xattr.h>
#include <stdarg.h>
#include <limits.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
#include "ft_ls.h"
// ----------------------

typedef struct s_output {
	struct dirent	*entry;
	struct s_output	*next;
} t_output;

typedef struct s_flag {
	bool l; // ok
	bool a; // ok
	bool R; // ok
	bool t; 
	bool r;
} t_flag;

// Definizione delle strutture
typedef struct FileInfo {
	char	*name;
	char	*owner;
	char	*group;
	char	*path;
	char	*symlink_target;
	mode_t	mode;
	uid_t	uid;
	gid_t	gid;
	off_t	size;
	time_t	mtime;
	nlink_t	nlink;
	bool	is_symlink;
} FileInfo;


//-----------------------

int		ft_snprintf(char *buffer, int size, const char *format, const char *str1, const char *str2);
int		ft_strlen(const char *str);
void	error(DIR *dir);
void	freeList(t_output *head);
void	flagset(t_flag *flags) ;
void	printList(t_output *head);
void ft_printf(const char *format, ...) ;
void ft_puthex(unsigned int n);
void ft_putnbr(int n);
void ft_putstr(char *str);
void ft_putchar(char c);
void	sortListAlphabetically(t_output **head);
void	display_permissions(mode_t mode, const char *full_path);
void	get_folder_paths(const char *path, char ***result, int *count, bool flagA);
void	ft_qsort(void *base, int nitems, int size, int (*compar)(const void *, const void *));
void	reverseList(t_output **head);
void	sortListByTime(t_output **head, const char *path);
char	*ft_strdup(const char *s1);
int	get_directory_blocks(const char *path, t_flag flags);
int	get_directory_size(const char *path);


#endif

