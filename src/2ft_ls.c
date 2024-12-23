#include "ft_ls.h"

void display_file_details(const char *path, const struct dirent *entry) {
	struct stat file_stat;
	char full_path[1024];

	ft_snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

	if (lstat(full_path, &file_stat) == -1) {
		perror("lstat");
		return;
	}

	display_permissions(file_stat.st_mode, full_path);
	ft_printf("%ld ", file_stat.st_nlink);

	struct passwd *pwd = getpwuid(file_stat.st_uid);
	struct group *grp = getgrgid(file_stat.st_gid);
	ft_printf("%s %s ", pwd ? pwd->pw_name : "?", grp ? grp->gr_name : "?");

	// Dimensione del file
	ft_printf("%ld ", file_stat.st_size);

	// Tempo di modifica
	char time_buf[20];
	strftime(time_buf, sizeof(time_buf), "%b %d %H:%M", localtime(&file_stat.st_mtime));
	ft_printf("%s ", time_buf);

	// Nome del file
	ft_printf("%s", entry->d_name);

	// Se il file è un link simbolico, mostra il percorso di destinazione
	if (S_ISLNK(file_stat.st_mode)) {
		char symlink_target[1024];
		int len = readlink(full_path, symlink_target, sizeof(symlink_target) - 1);
		if (len != -1) {
			symlink_target[len] = '\0'; // Assicurati che la stringa sia terminata
			ft_printf(" -> %s", symlink_target);
		} else {
			perror("readlink");
		}
	}

	ft_printf("\n"); // Fine della riga
}


void addToList(struct dirent *entry, t_output **output) {
    t_output *new_node = malloc(sizeof(t_output));
    if (!new_node) {
        perror("malloc");
        freeList(*output);
        exit(EXIT_FAILURE);
    }
    
    new_node->entry = malloc(sizeof(struct dirent));
    if (!new_node->entry) {
        perror("malloc");
        free(new_node);
        freeList(*output);
        exit(EXIT_FAILURE);
    }

    ft_memcpy(new_node->entry, entry, sizeof(struct dirent));
    new_node->next = NULL;

    if (*output == NULL) {
        *output = new_node;
    } else {
        t_output *current = *output;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
}

void list_directory(const char *path,t_flag flags) {
	DIR *dir = opendir(path);
	if (!dir) {
		perror("opendir");
		return;
	}

	t_output *output = NULL;
	if(flags.l)
		ft_printf("total: %d\n", get_directory_blocks(path, flags) );
	struct dirent *entry;

	while ((entry = readdir(dir)) != NULL) {
		addToList(entry, &output);
	}

		
	sortListAlphabetically(&output);
	if (flags.r)
		reverseList(&output);
	
	if (flags.t)
		sortListByTime(&output, path);
	
	t_output *head = output;
	while (output != NULL) {
		if (!flags.a && output->entry->d_name[0] == '.')
			output = output->next;
		else {
			if (flags.l)
			{
				display_file_details(path, output->entry);
			}
			else {
				ft_printf("%s   ", output->entry->d_name);
			}
			output = output->next;
		}
	}
	ft_printf("\n");
	closedir(dir);

	while (head != NULL) {
		t_output *tmp = head;
		head = head->next;
		free(tmp->entry);
		free(tmp);
	}
}

int main(int argc, char *argv[]) {
	
	t_flag flags;
	const char *path = ".";
	char **folders = NULL;
	int folder_count = 0;

	flagset(&flags);
	for (int i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			for (int j = 1; argv[i][j]; j++) {
				if (argv[i][j] == 'l')
					flags.l = 1;
				else if (argv[i][j] == 'a')
					flags.a = 1;
				else if (argv[i][j] == 'R')
				{
					flags.R = 1;
					get_folder_paths(path, &folders, &folder_count, flags.a);
				}
				else if (argv[i][j] == 't')
					{
						flags.t = 1;
						flags.r = 0;
					}
				else if (argv[i][j] == 'r')
					{
						flags.r = 1;
						flags.t = 0;
					}
				else {
					fprintf(stderr, "Usage: %s [-lartR] [file ...]\n", argv[0]);
					exit(EXIT_FAILURE);
				}
			}
		} else {
			path = argv[i];
		}
	}
	if (flags.R)
		ft_printf("%s:\n", path);	
	list_directory(path, flags);
	if (flags.R)
		ft_printf("\n");
	
	if (flags.R == 1) {
		if (flags.r)
			// Ordina le cartelle in ordine alfabetico inverso
			sort_paths_reverse(folders, folder_count);
		if (flags.t)
			// Ordina le cartelle per tempo di modifica
			ft_printf("Ordina per tempo di modifica\n");
		
		for (int i = 0; i < folder_count; i++) {
			if (!flags.a && (folders[i][0] == '.' && folders[i][1] == '/' && folders[i][2] == '.'))
			{
				free(folders[i]);
			}
			else {
                ft_printf("%s:\n", folders[i]);
                list_directory(folders[i], flags);
                ft_printf("\n");
                free(folders[i]);
            }
		}
		free(folders);
	}
	
	return 0;
}

