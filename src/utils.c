#include "ft_ls.h"

#define PATH_SIZE 1024

// Funzione ricorsiva per raccogliere i percorsi di cartelle e sottocartelle
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
			// Evitiamo di includere punti speciali "." e ".."
			if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
				// Costruiamo il percorso completo
				char new_path[PATH_SIZE];
				ft_snprintf(new_path, sizeof(new_path), "%s/%s", path, entry->d_name);
				(*result) = realloc(*result, sizeof(char *) * (++(*count)));
				(*result)[*count - 1] = strdup(new_path);
				get_folder_paths(new_path, result, count, flagA); // Ricorsivamente esplora la sottocartella
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
		return 0;  // Ritorna 0 in caso di errore
	}

	while ((entry = readdir(dir)) != NULL) {
		// Ignora "." e ".."
		if (!flags.a &&(entry->d_name[0] == '.') ) {
			continue;
		}
		
		char full_path[PATH_MAX];
		ft_snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

		// Ottieni informazioni sul file o directory
		if (lstat(full_path, &file_stat) == -1) {
			perror("lstat");
			continue;
		}

		// Somma i blocchi solo per i file e i link simbolici
		if (S_ISREG(file_stat.st_mode) || S_ISDIR(file_stat.st_mode)  || S_ISLNK(file_stat.st_mode)) {
			total_blocks += file_stat.st_blocks;
		}
	}

	closedir(dir);

	// Dividi i blocchi totali per 2 per ottenere il totale in KB come `ls -l`
	return total_blocks / 2;
}


int get_directory_size(const char *path) {
	struct stat file_stat;
	struct dirent *entry;
	DIR *dir = opendir(path);
	int total_size = 0;

	if (!dir) {
		perror("opendir");
		return 0;  // Ritornare 0 in caso di errore
	}

	while ((entry = readdir(dir)) != NULL) {
		// Escludi file nascosti se necessario (se non ci sono opzioni specifiche)
		if ((strcmp(entry->d_name, ".") == 0) || strcmp(entry->d_name, "..") == 0) {
			continue;
		}
		char full_path[PATH_MAX];
		ft_snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

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

void display_permissions(mode_t mode, const char *full_path) {
	char permissions[11] = {0}; // Stringa per i permessi
	int	is_symlink;
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

	ft_printf("%s ", permissions); // Stampa i permessi
}

// Funzione di confronto per ordinare in ordine crescente
int compare_int(const void *a, const void *b) {
	return (*(int *)a - *(int *)b);
}

int compare_paths(const void *a, const void *b) {
    const char *path1 = *(const char **)a;
    const char *path2 = *(const char **)b;
    return strcmp(path1, path2);
}


//funzione per il partizionamento dell'array
int partition(void *base, int low, int high, int size, int (*compar)(const void *, const void *)) {
    char *arr = (char *)base;
    char pivot[size];
    memcpy(pivot, arr + high * size, size); // Il pivot è l'elemento finale
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (compar(arr + j * size, pivot) <= 0) {
            i++;
            // Scambia arr[i] e arr[j]
            char temp[size];
            memcpy(temp, arr + i * size, size);
            memcpy(arr + i * size, arr + j * size, size);
            memcpy(arr + j * size, temp, size);
        }
    }

    // Posiziona il pivot al posto giusto
    char temp[size];
    memcpy(temp, arr + (i + 1) * size, size);
    memcpy(arr + (i + 1) * size, arr + high * size, size);
    memcpy(arr + high * size, temp, size);

    return i + 1; // Restituisce l'indice del pivot
}


// Funzione QuickSort ricorsiva
void quicksort(void *base, int low, int high, int size, int (*compar)(const void *, const void *)) {
    if (low < high) {
        int pi = partition(base, low, high, size, compar);
        quicksort(base, low, pi - 1, size, compar);  // Ordinamento della parte sinistra
        quicksort(base, pi + 1, high, size, compar); // Ordinamento della parte destra
    }
}

void ft_qsort(void *base, int nitems, int size, int (*compar)(const void *, const void *)) {
    quicksort(base, 0, nitems - 1, size, compar);
}

int compare_paths_by_time(const void *a, const void *b) {
    const char *path1 = *(const char **)a;
    const char *path2 = *(const char **)b;
    struct stat stat1, stat2;

    if (stat(path1, &stat1) != 0 || stat(path2, &stat2) != 0) {
        perror("Errore nell'accedere ai file");
        exit(EXIT_FAILURE);
    }

    if (stat1.st_mtime > stat2.st_mtime) {
        return -1;
    } else if (stat1.st_mtime < stat2.st_mtime) {
        return 1;
    } else {
        return strcmp(path1,path2);
    }
}

void sort_paths_by_time(char **paths, int count) {
	ft_qsort(paths, count, sizeof(char *), compare_paths_by_time);
}

// Funzione per ordinare un vettore di path in ordine alfabetico
void sort_paths_alphabetically(char **paths, int count) {
    ft_qsort(paths, count, sizeof(char *), compare_paths);
}


// Funzione per confrontare due stringhe in ordine inverso, usata per qsort
int compare_paths_reverse(const void *a, const void *b) {
    const char *path1 = *(const char **)a;
    const char *path2 = *(const char **)b;
	if (!strncmp(path1, path2, ft_strlen(path1)) || !strncmp(path2, path1, ft_strlen(path2)))
		return 0;
    return strcmp(path2, path1);
}


// Funzione per ordinare un vettore di path in ordine alfabetico inverso
void sort_paths_reverse(char **paths, int count) {
    ft_qsort(paths, count, sizeof(char *), compare_paths_reverse);
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
		ft_printf("PRINTLIST -> %s\n", current->entry->d_name); // Stampa il nome dell'entry
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
    int format_len = strlen(format);
    int str1_len = strlen(str1);
    int str2_len = strlen(str2);
    int required_size = format_len + str1_len + str2_len - 2; // '%s/%s' sostituisce due '%s'
    
    // Verifica se il buffer è abbastanza grande
    if (size < required_size + 1) { // +1 per il terminatore nullo
        errno = ENOMEM;
        perror("ft_snprintf: buffer troppo piccolo");
        return -1; // Indica errore
    }
    
    char *temp = malloc(required_size + 1); // +1 per '\0'
    if (!temp) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    // Costruzione manuale della stringa
    char *ptr = temp;
    for (const char *p = format; *p; ++p) {
        if (*p == '%' && *(p + 1) == 's') {
            const char *replacement = ptr == temp ? str1 : str2; // Prima sostituzione con str1, poi str2
            int len = strlen(replacement);
            memcpy(ptr, replacement, len);
            ptr += len;
            p++; // Salta 's' di '%s'
        } else {
            *ptr++ = *p;
        }
    }
    *ptr = '\0'; // Termina con '\0'

    // Copia il risultato nel buffer
    memcpy(buffer, temp, required_size + 1);
    free(temp);

    return required_size; // Restituisce la dimensione della stringa generata
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
            // Initialize the sorted list with the first element
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

                // Order by descending modification time
                if (current_stat.st_mtime > tmp_stat.st_mtime) {
                    break;
                }

                prev = tmp;
                tmp = tmp->next;
            }

            if (prev == NULL) {
                // Insert at the beginning
                current->next = newList;
                newList = current;
            } else {
                // Insert in the middle or at the end
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
        free(current->entry);  // Libera la memoria per `entry`
        free(current);  // Libera il nodo
        current = next;
    }
}
