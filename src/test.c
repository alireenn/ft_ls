#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include "ft_ls.h"



// Inizializza una struttura PathList
void init_path_list(PathList *list) {
    list->paths = malloc(10 * sizeof(char *));
    list->count = 0;
    list->capacity = 10;
}

// Aggiunge un percorso a PathList
void add_to_path_list(PathList *list, const char *path) {
    if (list->count >= list->capacity) {
        list->capacity *= 2;
        list->paths = realloc(list->paths, list->capacity * sizeof(char *));
    }
    list->paths[list->count++] = strdup(path);
}

// Libera la memoria di PathList
void free_path_list(PathList *list) {
    for (size_t i = 0; i < list->count; i++) {
        free(list->paths[i]);
    }
    free(list->paths);
}

// Funzione di confronto per ordinare i percorsi
static int compare_paths_by_time(const void *a, const void *b) {
    const char *path1 = *(const char **)a;
    const char *path2 = *(const char **)b;
    struct stat stat1, stat2;

    if (stat(path1, &stat1) != 0 || stat(path2, &stat2) != 0) {
        //perror("Errore nell'accedere ai file");
        //exit(EXIT_FAILURE);
    }

    // Ordina per tempo (decrescente)
    if (stat1.st_mtime > stat2.st_mtime) {
        return -1;
    } else if (stat1.st_mtime < stat2.st_mtime) {
        return 1;
    } else {
        // Ordina alfabeticamente in caso di timestamp uguali
        return strcmp(path1, path2);
    }
}

// Funzione principale per ottenere il vettore di cartelle ordinate
void get_sorted_folders(const char *path, PathList *result) {
    struct dirent **namelist;
    int n;

    // Leggi il contenuto della directory
    n = scandir(path, &namelist, NULL, NULL);
    if (n < 0) {
        perror("Errore nella lettura della directory");
        return;
    }

    // Array temporaneo per i percorsi completi
    char **sub_paths = malloc(n * sizeof(char *));
    int sub_count = 0;

    for (int i = 0; i < n; i++) {
        if (strcmp(namelist[i]->d_name, ".") == 0 || strcmp(namelist[i]->d_name, "..") == 0) {
            free(namelist[i]);
            continue;
        }

        char *full_path = malloc(strlen(path) + strlen(namelist[i]->d_name) + 2);
        sprintf(full_path, "%s/%s", path, namelist[i]->d_name);
        sub_paths[sub_count++] = full_path;
        free(namelist[i]);
    }
    free(namelist);

    // Ordina i percorsi per tempo e nome
    qsort(sub_paths, sub_count, sizeof(char *), compare_paths_by_time);

    // Processa le directory e aggiunge al risultato
    for (int i = 0; i < sub_count; i++) {
        struct stat path_stat;
        if (stat(sub_paths[i], &path_stat) == 0 && S_ISDIR(path_stat.st_mode)) {
            add_to_path_list(result, sub_paths[i]); // Aggiungi la directory
            get_sorted_folders(sub_paths[i], result); // Ricorsione per sottocartelle
        }
        free(sub_paths[i]);
    }

    free(sub_paths);
}
