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

#define PATH_SIZE 1024
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


// Struttura per contenere i percorsi
typedef struct {
    char **paths;
    size_t count;
    size_t capacity;
} PathList;

//-----------------------

void 	sort_paths_by_time(char **paths, int count);
void 	sort_paths_alphabetically(char **paths, int count);
void	sort_paths_reverse(char **paths, int count);
int		ft_snprintf(char *buffer, int size, const char *format, const char *str1, const char *str2);
int		ft_strlen(const char *str);
void	error(DIR *dir);
void	freeList(t_output *head);
void	flagset(t_flag *flags) ;
void	printList(t_output *head);
void 	ft_printf(const char *format, ...) ;
void	ft_puthex(unsigned int n);
void	ft_putnbr(int n);
void	addToList(struct dirent *entry, t_output **output);
void	ft_putstr(char *str);
void	ft_putchar(char c);
int		ft_strcmp(const char *s1, const char *s2);
void	sortListAlphabetically(t_output **head);
void	display_permissions(mode_t mode, const char *full_path);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
void	get_folder_paths(const char *path, char ***result, int *count, bool flagA);
void	ft_qsort(void *base, int nitems, int size, int (*compar)(const void *, const void *));
void	reverseList(t_output **head);
void	*ft_memcpy(void *dst, const void *src, size_t n);
void	sortListByTime(t_output **head, const char *path);
char	*ft_strdup(const char *s1);
void	*ft_memcpy(void *dst, const void *src, size_t n);
int		get_directory_blocks(const char *path, t_flag flags);
int		get_directory_size(const char *path);


// ----------------------
void get_sorted_folders(const char *path, PathList *result);
void init_path_list(PathList *list);

#endif


