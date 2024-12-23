#ifndef FT_LS
#define FT_LS
// ----------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
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

void	error(DIR *dir);
char	*ft_strdup(const char *s1);
int		ft_strlen(const char *str);
void	flagset(t_flag *flags) ;
void	printList(t_output *head);
size_t	get_directory_blocks(const char *path, t_flag flags);
void	sortListAlphabetically(t_output **head);
void	display_permissions(mode_t mode, const char *full_path);
void	get_folder_paths(const char *path, char ***result, int *count, bool flagA);
void	my_qsort(void *base, size_t nitems, size_t size, int (*compar)(const void *, const void *));
size_t get_directory_size(const char *path);
void	reverseList(t_output **head);
void	sortListByTime(t_output **head, const char *path);


#endif


