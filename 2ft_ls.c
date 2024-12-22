#include "ft_ls.h"

// Function to display file details (for -l option)
void display_file_details(const char *path, const struct dirent *entry) {
	struct stat file_stat;
	char full_path[1024];

	// Costruisce il percorso completo del file
	snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

	// Usa lstat per ottenere informazioni anche sui link simbolici
	if (lstat(full_path, &file_stat) == -1) {
		perror("lstat");
		return;
	}

	// Stampa i permessi
	display_permissions(file_stat.st_mode, full_path);

	// Numero di collegamenti
	printf("%ld ", file_stat.st_nlink);

	// Proprietario e gruppo
	struct passwd *pwd = getpwuid(file_stat.st_uid);
	struct group *grp = getgrgid(file_stat.st_gid);
	printf("%s %s ", pwd ? pwd->pw_name : "?", grp ? grp->gr_name : "?");

	// Dimensione del file
	printf("%ld ", file_stat.st_size);

	// Tempo di modifica
	char time_buf[20];
	strftime(time_buf, sizeof(time_buf), "%b %d %H:%M", localtime(&file_stat.st_mtime));
	printf("%s ", time_buf);

	// Nome del file
	printf("%s", entry->d_name);

	// Se il file è un link simbolico, mostra il percorso di destinazione
	if (S_ISLNK(file_stat.st_mode)) {
		char symlink_target[1024];
		ssize_t len = readlink(full_path, symlink_target, sizeof(symlink_target) - 1);
		if (len != -1) {
			symlink_target[len] = '\0'; // Assicurati che la stringa sia terminata
			printf(" -> %s", symlink_target);
		} else {
			perror("readlink");
		}
	}

	printf("\n"); // Fine della riga
}


void addToList(struct dirent *entry, t_flag flags, t_output **output) {
	// Crea un nuovo nodo per la lista concatenata
	t_output *new_node = malloc(sizeof(t_output));
	if (!new_node) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	new_node->entry = malloc(sizeof(struct dirent));
	if (!new_node->entry) {
		perror("malloc");
		free(new_node);
		exit(EXIT_FAILURE);
	}
	memcpy(new_node->entry, entry, sizeof(struct dirent));
	new_node->next = NULL;

	// Aggiungi il nuovo nodo alla lista concatenata
	if (*output == NULL) {
		// Se la lista è vuota, il nuovo nodo diventa la testa
		*output = new_node;
	} else {
		// Altrimenti, cerca la coda della lista e aggiungi il nuovo nodo
		t_output *current = *output;
		while (current->next != NULL) {
			current = current->next;
		}
		current->next = new_node;
	}
}


// Function to list directory contents
void list_directory(const char *path, t_flag flags) {
	DIR *dir = opendir(path);
	if (!dir) {
		perror("opendir");
		return;
	}

	printf("total: %ld\n", get_directory_blocks(path, flags) );
	struct dirent *entry;
	t_output *output = NULL; // Inizializza la lista concatenata come vuota

	//printList(output);
	while ((entry = readdir(dir)) != NULL) {
		addToList(entry, flags, &output);
	}
	sortListAlphabetically(&output);
	//if (flags.r)
	while (output != NULL) {
		if (!flags.a && output->entry->d_name[0] == '.')
		{
			//while (output->entry->d_name[0] == '.' && output->next != NULL)
				output = output->next;
		}
		else {
			if (flags.l)
			{
				display_file_details(path, output->entry);
			}
			else {
				if (flags.R)
					printf("\t");
				printf("%s\n", output->entry->d_name);
			}
			output = output->next;
		}
	}

	closedir(dir);
	// Elaborazione della lista concatenata
	t_output *current = output;

	// Libera la memoria allocata
	while (output) {
		t_output *tmp = output;
		output = output->next;
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
		if (strcmp(argv[i], "-a") == 0) {
			flags.a = 1;
		} else if (strcmp(argv[i], "-l") == 0) {
			flags.l = 1;
		} else if (strcmp(argv[i], "-R") == 0) {
			flags.R = 1;
			get_folder_paths(path, &folders, &folder_count, flags.a);
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
			if (!flags.a && (folders[i][0] == '.' && folders[i][1] == '/' && folders[i][2] == '.'))
				free (folders[i]);
			else {
				printf("%s:\n", folders[i]);
				list_directory(folders[i], flags);
				printf("\n");
				free(folders[i]);
			}
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
