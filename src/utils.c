#include "ft_ls.h"

void get_folder_paths(const char *path, char ***result, int *count, bool flagA) {
	struct dirent *entry;
	DIR *dp;

	dp = opendir(path);
	if (dp == NULL) {
		perror("opendir");
		return;
	}

	while ((entry = readdir(dp))) {
		if (entry->d_type == DT_DIR) {
			if (ft_strcmp(entry->d_name, ".") != 0 && ft_strcmp(entry->d_name, "..") != 0) {
				//percorso completo
				char new_path[PATH_SIZE];
				ft_snprintf(new_path, sizeof(new_path), "%s/%s", path, entry->d_name);
				(*result) = realloc(*result, sizeof(char *) * (++(*count)));
				(*result)[*count - 1] = ft_strdup(new_path);
				get_folder_paths(new_path, result, count, flagA);
			}
		}
	}

	closedir(dp);
}

int get_directory_blocks(const char *path, t_flag flags) {
	struct stat file_stat;
	struct dirent *entry;
	DIR *dir = opendir(path);
	int total_blocks = 0;

	if (!dir) {
		perror("opendir");
		return 0;
	}

	while ((entry = readdir(dir)) != NULL) {
		if (!flags.a &&(entry->d_name[0] == '.') ) {
			continue;
		}
		
		char full_path[PATH_MAX];
		ft_snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
		if (lstat(full_path, &file_stat) == -1) {
			perror("lstat");
			continue;
		}
		total_blocks += file_stat.st_blocks;
	}

	closedir(dir);
	return total_blocks / 2;
}

int get_directory_size(const char *path) {
	struct stat file_stat;
	struct dirent *entry;
	DIR *dir = opendir(path);
	int total_size = 0;

	if (!dir) {
		perror("opendir");
		return 0;
	}

	while ((entry = readdir(dir)) != NULL) {
		if ((ft_strcmp(entry->d_name, ".") == 0) || ft_strcmp(entry->d_name, "..") == 0)
			continue;
		char full_path[PATH_MAX];
		ft_snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
		if (lstat(full_path, &file_stat) == -1) 
		{
			perror("stat");
			continue;
		}
		if (S_ISREG(file_stat.st_mode)) {
			total_size += file_stat.st_size;
		} else if (S_ISDIR(file_stat.st_mode)) {
			total_size += get_directory_size(full_path);
		}
	}

	closedir(dir);
	return total_size;
}

char *formatTime(char *time)
{
	int i;
	int j = 0;
	char *ret;

	ret = malloc(sizeof(char) * 17);
	for(i = 0; i < ft_strlen(time); i++)
	{
		if ((i >= 0 && i <= 3) || (i >= 16 && i <= ft_strlen(time)))
			continue ;
		ret[j] = time[i];
		j++;
	}
	ret[j] = '\0';
	return(ret);
}


void display_permissions(mode_t mode, const char *full_path) {
	char permissions[11] = {0};
	int	is_symlink;
	char	target[PATH_MAX] = {0};

	if (full_path == NULL) {
		permissions[0] = '-';
	} else {
		is_symlink = readlink(full_path, target, sizeof(target) - 1);
		if (is_symlink >= 0) {
			permissions[0] = 'l'; // Link simbolico
		} else if (S_ISDIR(mode)) {
			permissions[0] = 'd'; // Directory
		} else {
			permissions[0] = '-'; // File regolare
		}
	}
	// Permessi per l'utente
	permissions[1] = (mode & S_IRUSR) ? 'r' : '-';
	permissions[2] = (mode & S_IWUSR) ? 'w' : '-';
	permissions[3] = (mode & S_IXUSR) ? 'x' : '-';

	// Permessi per il gruppo
	permissions[4] = (mode & S_IRGRP) ? 'r' : '-';
	permissions[5] = (mode & S_IWGRP) ? 'w' : '-';
	permissions[6] = (mode & S_IXGRP) ? 'x' : '-';

	// Permessi per gli altri
	permissions[7] = (mode & S_IROTH) ? 'r' : '-';
	permissions[8] = (mode & S_IWOTH) ? 'w' : '-';
	permissions[9] = (mode & S_IXOTH) ? 'x' : '-';

	permissions[10] = '\0'; // Terminatore della stringa

	ft_printf("%s ", permissions); // Stampa i permessi
}



void	reverseList(t_output **head)
{
	t_output *prev = NULL;
	t_output *current = *head;
	t_output *next = NULL;

	while (current != NULL) {
		next = current->next;
		current->next = prev;
		prev = current;
		current = next;
	}
	*head = prev;
}

int ft_snprintf(char *buffer, int size, const char *format, const char *str1, const char *str2) {
    int format_len = ft_strlen(format);
    int str1_len = ft_strlen(str1);
    int str2_len = ft_strlen(str2);
    int required_size = format_len + str1_len + str2_len - 2;
    
    if (size < required_size + 1) {
        errno = ENOMEM;
        perror("ft_snprintf: buffer troppo piccolo");
        return -1;
    }
    
    char *temp = malloc(required_size + 1);
    if (!temp) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    char *ptr = temp;
    for (const char *p = format; *p; ++p) {
        if (*p == '%' && *(p + 1) == 's') {
            const char *replacement = ptr == temp ? str1 : str2;
            int len = ft_strlen(replacement);
            ft_memcpy(ptr, replacement, len);
            ptr += len;
            p++;
        } else {
            *ptr++ = *p;
        }
    }
    *ptr = '\0';
    ft_memcpy(buffer, temp, required_size + 1);
    free(temp);

    return required_size;
}

void sortListByTime(t_output **head, const char *path)
{
    t_output *current = *head;
    t_output *newList = NULL;

    while (current != NULL) {
        t_output *next = current->next;
        char current_path[PATH_MAX];
        ft_snprintf(current_path, sizeof(current_path), "%s/%s", path, current->entry->d_name);

        struct stat current_stat;
        if (lstat(current_path, &current_stat) == -1) {
            perror("lstat");
            current = next;
            continue;
        }

        if (newList == NULL) {
            newList = current;
            newList->next = NULL;
        } else {
            t_output *tmp = newList;
            t_output *prev = NULL;

            while (tmp != NULL) {
                char tmp_path[PATH_MAX];
                ft_snprintf(tmp_path, sizeof(tmp_path), "%s/%s", path, tmp->entry->d_name);

                struct stat tmp_stat;
                if (lstat(tmp_path, &tmp_stat) == -1) {
                    perror("lstat");
                    break;
                }

                if (current_stat.st_mtime > tmp_stat.st_mtime) {
                    break;
                }

                prev = tmp;
                tmp = tmp->next;
            }

            if (prev == NULL) {
                current->next = newList;
                newList = current;
            } else {
                prev->next = current;
                current->next = tmp;
            }
        }
        current = next;
    }
    *head = newList;
}

void freeList(t_output *head) {
    t_output *current = head;
    t_output *next;

    while (current != NULL) {
        next = current->next;
        free(current->entry);
        free(current);
        current = next;
    }
}
