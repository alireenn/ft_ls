#include "ft_ls.h"

int ft_scandir(const char *dir_name, struct dirent ***namelist) {
    DIR *dir;
    struct dirent *entry;
    struct dirent **list = NULL;
    size_t count = 0;
    size_t capacity = 10;

    list = malloc(capacity * sizeof(struct dirent *));
    if (list == NULL) {
        perror("malloc");
        return -1;
    }

    dir = opendir(dir_name);
    if (dir == NULL) {
        perror("opendir");
        free(list);
        return -1;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (count >= capacity) {
            capacity *= 2;
            struct dirent **new_list = malloc(capacity * sizeof(struct dirent *));
            if (new_list == NULL) {
                perror("malloc");
                for (size_t i = 0; i < count; i++) {
                    free(list[i]);
                }
                free(list);
                closedir(dir);
                return -1;
            }

            ft_memcpy(new_list, list, count * sizeof(struct dirent *));
            free(list);
            list = new_list;
        }

        list[count] = malloc(sizeof(struct dirent));
        if (list[count] == NULL) {
            perror("malloc");
            for (size_t i = 0; i < count; i++) {
                free(list[i]);
            }
            free(list);
            closedir(dir);
            return -1;
        }
        memcpy(list[count], entry, sizeof(struct dirent));
        count++;
    }
    *namelist = list;
    closedir(dir);

    return count;
}

void init_path_list(PathList *list) {
    list->paths = malloc(10 * sizeof(char *));
    list->count = 0;
    list->capacity = 10;
}

void add_to_path_list(PathList *list, const char *path) {
    if (list->count >= list->capacity) {
        list->capacity *= 2;
        list->paths = realloc(list->paths, list->capacity * sizeof(char *));
    }
    list->paths[list->count++] = strdup(path);
}

void free_path_list(PathList *list) {
    for (size_t i = 0; i < list->count; i++) {
        free(list->paths[i]);
    }
    free(list->paths);
}

static int compare_paths_by_time(const void *a, const void *b) {
    const char *path1 = *(const char **)a;
    const char *path2 = *(const char **)b;
    struct stat stat1, stat2;

    if (stat(path1, &stat1) != 0 || stat(path2, &stat2) != 0) {
        return(0);
    }
    if (stat1.st_mtime > stat2.st_mtime) {
        return -1;
    } 
    else if (stat1.st_mtime < stat2.st_mtime) {
        return 1;
    } 
    else
        return ft_strlowcmp(path1, path2);
}

void get_sorted_folders(const char *path, PathList *result) {
    struct dirent **namelist;
    int n;

    n = ft_scandir(path, &namelist);
    if (n < 0) {
        perror("Errore nella lettura della directory");
        return;
    }
    
    char **sub_paths = malloc(n * sizeof(char *));
    int sub_count = 0;
    for (int i = 0; i < n; i++) {
        if (strcmp(namelist[i]->d_name, ".") == 0 || strcmp(namelist[i]->d_name, "..") == 0) {
            free(namelist[i]);
            continue;
        }
        char *full_path = malloc(strlen(path) + strlen(namelist[i]->d_name) + 2);
        ft_sprintf(full_path, "%s/%s", path, namelist[i]->d_name);
        sub_paths[sub_count++] = full_path;
        free(namelist[i]);
    }
    free(namelist);
    ft_qsort(sub_paths, sub_count, sizeof(char *), compare_paths_by_time);

    for (int i = 0; i < sub_count; i++) {
        struct stat path_stat;
        if (stat(sub_paths[i], &path_stat) == 0 && S_ISDIR(path_stat.st_mode)) {
            add_to_path_list(result, sub_paths[i]);
            get_sorted_folders(sub_paths[i], result);
        }
        free(sub_paths[i]);
    }
    free(sub_paths);
}
