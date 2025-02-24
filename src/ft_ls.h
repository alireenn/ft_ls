

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
typedef struct s_PathList{
    char **paths;
    size_t count;
    size_t capacity;
} PathList;

//-----------------------

void 	sort_paths_by_time(char **paths, int count);
void 	sort_paths_alphabetically(char **paths, int count);
void	sort_paths_reverse(char **paths, int count);
void	sortListByTime(t_output **head, const char *path);
int		compare_paths(const void *a, const void *b);

void	add_to_path_list(PathList *list, const char *path);
char	*ft_strcpy(char *dest, const char *src);
int		get_directory_blocks(const char *path, t_flag flags);
int		get_directory_size(const char *path);
int		ft_sprintf(char *buffer, const char *format, ...);
int		ft_strlowcmp(const char *s1, const char *s2);
int		ft_strlowncmp(const char *s1, const char *s2, size_t size);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strtrim(char const *s1);
int		ft_strcmp(const char *s1, const char *s2);
int		ft_snprintf(char *buffer, int size, const char *format, const char *str1, const char *str2);
int		ft_strlen(const char *str);
void	error(DIR *dir);
void	freeList(t_output *head);
void	flagset(t_flag *flags) ;
void	printList(t_output *head);
char	*formatTime(char *time);
void 	ft_printf(const char *format, ...) ;
void	ft_puthex(unsigned int n);
void	ft_putnbr(int n);
void	addToList(struct dirent *entry, t_output **output);
void	ft_putstr(char *str);
void	freeMat(char **mat, int count);
void	ft_putchar(char c);
void	ft_tolower(char *str);
void	sortListAlphabetically(t_output **head);
void	display_permissions(mode_t mode, const char *full_path);
void	get_folder_paths(const char *path, char ***result, int *count, bool flagA);
void	reverseList(t_output **head);
void	*ft_memcpy(void *dst, const void *src, size_t n);
char	*ft_strdup(const char *s1);
void	*ft_memcpy(void *dst, const void *src, size_t n);
void	*ft_realloc(void *ptr, size_t size);
void ft_qsort(void *base, size_t n_items, size_t item_size, int (*compare)(const void *, const void *));
void	*ft_memcpy(void *dst, const void *src, size_t n);
int		compare_paths_tool(const void *a, const void *b);


// ----------------------
void get_sorted_folders(const char *path, PathList *result);
void init_path_list(PathList *list);

#endif


