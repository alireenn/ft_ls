#include "ft_ls.h"

void swap(void *a, void *b, size_t size) {
    char temp[size];
    memcpy(temp, a, size);
    memcpy(a, b, size);
    memcpy(b, temp, size);
}

void ft_qsort(void *base, size_t n_items, size_t item_size, int (*compare)(const void *, const void *)) {
    if (n_items < 2) return;

    char *array = (char *)base;
    char *pivot = array + (n_items - 1) * item_size;

    size_t i = 0;

    for (size_t j = 0; j < n_items - 1; ++j) {
        char *current = array + j * item_size;
        if (compare(current, pivot) <= 0) {
            char *smaller = array + i * item_size;
            swap(smaller, current, item_size);
            i++;
        }
    }
    char *pivot_position = array + i * item_size;
    swap(pivot_position, pivot, item_size);
    ft_qsort(array, i, item_size, compare);                                       //sinistra
    ft_qsort(pivot_position + item_size, n_items - i - 1, item_size, compare);    //destra
}

int compare_paths_by_time(const void *a, const void *b) {

    const char *path1 = *(const char **)a;
    const char *path2 = *(const char **)b;
    struct stat stat1, stat2;


    if (stat(path1, &stat1) != 0 || stat(path2, &stat2) != 0) {
        perror("Errore nell'accedere ai file");
        exit(EXIT_FAILURE);
    }

    if (!ft_strlowncmp(path1, path2, ft_strlen(path1) -2) || !ft_strlowncmp(path1, path2, ft_strlen(path2)-2))
        return 0;

    if (stat1.st_mtime > stat2.st_mtime) 
        return -1;
    else if (stat1.st_mtime < stat2.st_mtime) 
        return 1;
    return ft_strcmp(path1,path2);

}

void sort_paths_by_time(char **paths, int count) {
	
	ft_qsort(paths, count, sizeof(char *), compare_paths_by_time);
    ft_qsort(paths, count, sizeof(char *), compare_paths_tool);
}

int compare_paths(const void *a, const void *b) {
    const char *path1 = *(const char **)a;
    const char *path2 = *(const char **)b;
   
    char * aux1 = ft_strtrim(path1);
    char * aux2 = ft_strtrim(path2);
    int aux3 = ft_strlowcmp(aux1, aux2);
    free(aux1);
    free(aux2);
    return aux3;
}

void sort_paths_alphabetically(char **paths, int count) {
    ft_qsort(paths, count, sizeof(char *), compare_paths);
}


int compare_paths_tool(const void *a, const void *b) {
    const char *path1 = *(const char **)a;
    const char *path2 = *(const char **)b;
    if (!ft_strlowncmp(path1, path2, ft_strlen(path1) -2) || !ft_strlowncmp(path1, path2, ft_strlen(path2)-2))
        return 1;
    return 0;
}

int compare_paths_reverse(const void *a, const void *b) {
    const char *path1 = *(const char **)a;
    const char *path2 = *(const char **)b;

    char *tmp2 = ft_strtrim(path2);
    char *tmp1 = ft_strtrim(path1);
    int x = ft_strlowcmp(tmp2, tmp1); 
    free(tmp1);
    free(tmp2);
    return x;

}

void sort_paths_reverse(char **paths, int count) {
    ft_qsort(paths, count, sizeof(char *), compare_paths_reverse);
    ft_qsort(paths, count, sizeof(char *), compare_paths_tool);
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
            char *tmp1 = ft_strtrim(ptr1->entry->d_name);
            char *tmpnext = ft_strtrim(ptr1->next->entry->d_name);
			if (ft_strlowcmp(tmp1, tmpnext) > 0) {
				struct dirent *temp = ptr1->entry;
				ptr1->entry = ptr1->next->entry;
				ptr1->next->entry = temp;

				swapped = true;
			}
            free(tmp1);
            free(tmpnext);
			ptr1 = ptr1->next;
		}
		lptr = ptr1;
	} while (swapped);
}

