#include "ft_ls.h"


//funzione per il partizionamento dell'array
int partition(void *base, int low, int high, int size, int (*compar)(const void *, const void *)) {
    char *arr = (char *)base;
    char pivot[size];
    ft_memcpy(pivot, arr + high * size, size); // Il pivot è l'elemento finale
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (compar(arr + j * size, pivot) <= 0) {
            i++;
            // Scambia arr[i] e arr[j]
            char temp[size];
            ft_memcpy(temp, arr + i * size, size);
            ft_memcpy(arr + i * size, arr + j * size, size);
            ft_memcpy(arr + j * size, temp, size);
        }
    }

    // Posiziona il pivot al posto giusto
    char temp[size];
    ft_memcpy(temp, arr + (i + 1) * size, size);
    ft_memcpy(arr + (i + 1) * size, arr + high * size, size);
    ft_memcpy(arr + high * size, temp, size);

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
        return ft_strcmp(path1,path2);
    }
}

void sort_paths_by_time(char **paths, int count) {
	
	ft_qsort(paths, count, sizeof(char *), compare_paths_by_time);
}

// Funzione per ordinare un vettore di path in ordine alfabetico
void sort_paths_alphabetically(char **paths, int count) {
    ft_qsort(paths, count, sizeof(char *), compare_paths);
}


int compare_paths_reverse(const void *a, const void *b) {
    const char *path1 = *(const char **)a;
    const char *path2 = *(const char **)b;
	if (!ft_strlowncmp(path1, path2, ft_strlen(path1)) || !ft_strlowncmp(path2, path1, ft_strlen(path2)))
		return 0;
    return ft_strcmp(path2, path1);
}

void sort_paths_reverse(char **paths, int count) {
    ft_qsort(paths, count, sizeof(char *), compare_paths_reverse);
}

void sortListAlphabetically(t_output **head) {
	if (!head || !*head) {
		return;
	}

	bool swapped;
	t_output *ptr1;
	t_output *lptr = NULL;

	do {
		swapped = false;
		ptr1 = *head;

		while (ptr1->next != lptr) {
			if (ft_strlowcmp(ptr1->entry->d_name, ptr1->next->entry->d_name) > 0) {
				struct dirent *temp = ptr1->entry;
				ptr1->entry = ptr1->next->entry;
				ptr1->next->entry = temp;

				swapped = true;
			}
			ptr1 = ptr1->next;
		}
		lptr = ptr1;
	} while (swapped);
}

