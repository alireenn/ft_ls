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


size_t get_directory_size(const char *path) {
	struct stat file_stat;
	struct dirent *entry;
	DIR *dir = opendir(path);
	size_t total_size = 0;

	if (!dir) {
		perror("opendir");
		return 0;  // Ritornare 0 in caso di errore
	}

	while ((entry = readdir(dir)) != NULL) {
		// Escludi file nascosti se necessario (se non ci sono opzioni specifiche)
		if (entry->d_name[0] == '.' && entry->d_name[1] != '\0') {
			continue;
		}
		char full_path[PATH_MAX];
		snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

		// Ottieni informazioni sul file o directory
		if (lstat(full_path, &file_stat) == -1) {
			perror("stat");
			continue;
		}

		if (S_ISREG(file_stat.st_mode)) {
			// Se è un file regolare, aggiungi la sua dimensione
			total_size += file_stat.st_size;
		} else if (S_ISDIR(file_stat.st_mode)) {
			// Se è una directory, richiamalo ricorsivamente
			total_size += get_directory_size(full_path);
		}
	}

	closedir(dir);
	return total_size;
}


// Function to display permissions
void display_permissions(mode_t mode, const char *full_path) {
	char permissions[11] = {0}; // Stringa per i permessi
	ssize_t	is_symlink;
	char	target[PATH_MAX] = {0};

	// Controlla se è un link simbolico
	if (full_path == NULL) {
		permissions[0] = '-'; // Default se il percorso non è valido
	} else {
		is_symlink = readlink(full_path, target, sizeof(target) - 1); // Controlla se è un link
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

	printf("%s ", permissions); // Stampa i permessi
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

void sortListAlphabetically(t_output **head) {
	if (!head || !*head) {
		return; // Lista vuota o nulla
	}

	bool swapped;
	t_output *ptr1;
	t_output *lptr = NULL; // Indica l'ultima posizione già ordinata

	do {
		swapped = false;
		ptr1 = *head;

		while (ptr1->next != lptr) {
			if (strcmp(ptr1->entry->d_name, ptr1->next->entry->d_name) > 0) {
				// Scambia i nodi
				struct dirent *temp = ptr1->entry;
				ptr1->entry = ptr1->next->entry;
				ptr1->next->entry = temp;

				swapped = true;
			}
			ptr1 = ptr1->next;
		}
		lptr = ptr1; // Aggiorna l'ultima posizione ordinata
	} while (swapped);
}
void printList(t_output *head) {
	t_output *current = head; // Puntatore al nodo corrente
	while (current != NULL) {
		printf("PRINTLIST -> %s\n", current->entry->d_name); // Stampa il nome dell'entry
		current = current->next; // Passa al prossimo nodo
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