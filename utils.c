#include "ft_ls.h"

#define PATH_SIZE 1024

// Funzione ricorsiva per raccogliere i percorsi di cartelle e sottocartelle
void get_folder_paths(const char *path, char ***result, int *count) {
	struct dirent *entry;
	DIR *dp;

	dp = opendir(path);
	if (dp == NULL) {
		perror("opendir");
		return;
	}

	while ((entry = readdir(dp))) {
		if (entry->d_type == DT_DIR) {
			// Evitiamo di includere punti speciali "." e ".."
			if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
				// Costruiamo il percorso completo
				char new_path[PATH_SIZE];
				snprintf(new_path, sizeof(new_path), "%s/%s", path, entry->d_name);
				(*result) = realloc(*result, sizeof(char *) * (++(*count)));
				(*result)[*count - 1] = strdup(new_path);
				get_folder_paths(new_path, result, count); // Ricorsivamente esplora la sottocartella
			}
		}
	}

	closedir(dp);
}
// Function to display permissions
void display_permissions(mode_t mode) {
	char permissions[12] = {0}; // Initialize the array to prevent undefined behavior
	permissions[0] = (S_ISDIR(mode)) ? 'd' : 
                     (S_ISLNK(mode)) ? 'l' : '-';
	permissions[0] = (S_ISDIR(mode)) ? 'd' : '-';
	permissions[1] = (mode & S_IRUSR) ? 'r' : '-';
	permissions[2] = (mode & S_IWUSR) ? 'w' : '-';
	permissions[3] = (mode & S_IXUSR) ? 'x' : '-';
	permissions[4] = (mode & S_IRGRP) ? 'r' : '-';
	permissions[5] = (mode & S_IWGRP) ? 'w' : '-';
	permissions[6] = (mode & S_IXGRP) ? 'x' : '-';
	permissions[7] = (mode & S_IROTH) ? 'r' : '-';
	permissions[8] = (mode & S_IWOTH) ? 'w' : '-';
	permissions[9] = (mode & S_IXOTH) ? 'x' : '-';
	permissions[10] = '\0';
	printf("%s ", permissions);
}

// Funzione di confronto per ordinare in ordine crescente
int compare_int(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

// Implementazione semplice di qsort
void my_qsort(void *base, size_t nitems, size_t size, int (*compar)(const void *, const void *)) {
    char *arr = (char *)base;
    for (size_t i = 0; i < nitems - 1; i++) {
        for (size_t j = i + 1; j < nitems; j++) {
            char *elem_i = arr + i * size;
            char *elem_j = arr + j * size;
            if (compar(elem_i, elem_j) > 0) {
                // Scambia gli elementi
                char temp[size];
                memcpy(temp, elem_i, size);
                memcpy(elem_i, elem_j, size);
                memcpy(elem_j, temp, size);
            }
        }
    }
}

void error(DIR *dir)
{
	if (dir != NULL)
		closedir(dir);
	perror("Bad malloc\n");
	exit (1);
}

int	ft_strlen(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*ft_strdup(const char *s1)
{
	int		i;
	int		len;
	char	*dest;

	i = 0;
	len = ft_strlen(s1);
	dest = malloc((len + 1) * sizeof(char));
	while (s1[i] != '\0')
	{
		dest[i] = s1[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}
void flagset(t_flag *flags)
{
	flags->a = false;
	flags->l = false;
	flags->R = false;
	flags->r = false;
	flags->t = false;
}