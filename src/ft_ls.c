
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
	ft_printf("%d ", (int)file_stat.st_nlink);

	struct passwd *pwd = getpwuid(file_stat.st_uid);
	struct group *grp = getgrgid(file_stat.st_gid);
	ft_printf("%s %s ", pwd ? pwd->pw_name : "?", grp ? grp->gr_name : "?");

	// Dimensione del file
	ft_printf("%d ", file_stat.st_size); //capire

	// Tempo di modifica
	char *time_buf;
	time_buf = formatTime(ctime(&file_stat.st_mtime));
	ft_printf("%s", time_buf);

	// Nome del file
	ft_printf(" %s", entry->d_name);

	if (S_ISLNK(file_stat.st_mode)) {
		char symlink_target[1024];
		int len = readlink(full_path, symlink_target, sizeof(symlink_target) - 1);
		if (len != -1) {
			symlink_target[len] = '\0';
			ft_printf(" -> %s", symlink_target);
		} else {
			perror("readlink");
		}
	}
		ft_printf("\n");
		free(time_buf);
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
	
	if (flags.t)
		sortListByTime(&output, path);
	if (flags.r)
		reverseList(&output);
	
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
				ft_printf("%s  ", output->entry->d_name);
			}
			output = output->next;
		}
	}
	if (!flags.l)
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
			if(argv[i][1] == 0)
			{
				
				fprintf(stderr, "Usage: %s [-lartR] [file ...]\n", argv[0]);
				exit(EXIT_FAILURE);
			}
			for (int j = 1; argv[i][j]; j++) {
				if (argv[i][j] == 'l')
					flags.l = 1;
				else if (argv[i][j] == 'a')
					flags.a = 1;
				else if (argv[i][j] == 'R')
					flags.R = 1;					
				else if (argv[i][j] == 'r')
				{
					if (!flags.t)
						flags.r = 1;
				}
				else if (argv[i][j] == 't')
				{ 
					flags.t = 1;
					flags.r = 0;
				}
				else {
					fprintf(stderr, "Usage: %s [-lartR] [file ...]\n", argv[0]);
					exit(EXIT_FAILURE);
				}
			}
		}
		else{
			path = argv[i];
		}
	}

	PathList result;
	result.capacity=0;
	result.count=0;
	result.paths=0;
	if (flags.R)
	{
		get_folder_paths(path, &folders, &folder_count, flags.a);
		sort_paths_alphabetically(folders, folder_count);
	}
	if (flags.t && flags.R)
	{
		init_path_list(&result);
		get_sorted_folders(path, &result);
		freeMat(folders, folder_count);
		folders = result.paths;
		folder_count = result.count;
	}
	
	if (flags.r)
		sort_paths_reverse(folders, folder_count);
	if (flags.R)
		ft_printf("%s:\n", path);	
	list_directory(path, flags);
	if (flags.R && !flags.l)
		ft_printf("\n");
	
	if (flags.R == 1) {
		for (int i = 0; i < folder_count; i++) {
			if (!flags.a && (folders[i][0] == '.' && folders[i][1] == '/' && folders[i][2] == '.'))
				free(folders[i]);
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

