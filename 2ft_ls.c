#include "ft_ls.h"

// Function to display file details (for -l option)
void display_file_details(const char *path, const struct dirent *entry) {
	struct stat file_stat;
	char full_path[1024];

	snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

	if (lstat(full_path, &file_stat) == -1) {
		perror("stat");
		return;
	}

	display_permissions(file_stat.st_mode);

	printf("%ld ", file_stat.st_nlink);
	//file_stat.
	struct passwd *pwd = getpwuid(file_stat.st_uid);
	struct group *grp = getgrgid(file_stat.st_gid);

	printf("%s %s ", pwd ? pwd->pw_name : "?", grp ? grp->gr_name : "?");
	printf("%ld ", file_stat.st_size);

	char time_buf[20];
	strftime(time_buf, sizeof(time_buf), "%b %d %H:%M", localtime(&file_stat.st_mtime));
	printf("%s ", time_buf);

	printf("%s\n", entry->d_name);
}

// Function to list directory contents
void list_directory(const char *path, t_flag flags) {
	DIR *dir = opendir(path);
	if (!dir) {
		perror("opendir");
		return;
	}
	
	struct dirent *entry;
	while ((entry = readdir(dir)) != NULL) {
		if (!flags.a && entry->d_name[0] == '.')
			continue;

		if (flags.l) {
			display_file_details(path, entry);
		} else {
			if (flags.R == true)
				printf("\t");
			printf("%s\n", entry->d_name);
		}
	}

	closedir(dir);
}

int main(int argc, char *argv[]) {
	
	t_flag flags;
	const char *path = ".";
	char **folders = NULL;
	int folder_count = 0;

	flagset(&flags);
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-a") == 0) {
			flags.a = 1;
		} else if (strcmp(argv[i], "-l") == 0) {
			flags.l = 1;
		} else if (strcmp(argv[i], "-R") == 0) {
			flags.R = 1;
			get_folder_paths(path, &folders, &folder_count);
		} else {
			path = argv[i];
		}
	}
	if (flags.R)
		printf("%s:\n", path);	
	list_directory(path, flags);
	if (flags.R)
		printf("\n");	
	
	if (flags.R == 1) {
		for (int i = 0; i < folder_count; i++) {
			printf("%s:\n", folders[i]);
			list_directory(folders[i], flags);
			printf("\n");
			free(folders[i]);
		}
		free(folders);
	}

	return 0;
}

/*

int main() {
    
    

    const char *root_path = "."; // Percorso della directory corrente

    

    // Stampa i percorsi trovati
    for (int i = 0; i < folder_count; i++) {
        printf("%s\n", folders[i]);
          // Libera la memoria allocata per ciascun percorso
    }

      // Libera la memoria allocata per l'elenco dei percorsi

    return 0;
}

*/
