// C
// Program care primeste ca argument in linia de comanda un director si sterge (recursiv) toata arborescenta cu originea in el.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

void delete_dir(char *dir_name)
{
    DIR *dir;
    struct dirent *entry;
    struct stat statbuf;
    char *path;

    if ((dir = opendir(dir_name)) == NULL)
    {
        perror("opendir");
        exit(1);
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        char *path = malloc(strlen(dir_name) + strlen(entry->d_name) + 2);
        if (path == NULL)
        {
            perror("malloc");
            closedir(dir);
            exit(1);
        }
        strcpy(path, dir_name);
        strcat(path, "/");
        strcat(path, entry->d_name);
        if (lstat(path, &statbuf) == -1)
        {
            perror("lstat");
            free(path);
            closedir(dir);
            exit(1);
        }

        if (S_ISDIR(statbuf.st_mode))
            delete_dir(path);
        else
            unlink(path);
        free(path);
    }

    closedir(dir);
    rmdir(dir_name);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <nume_dir>\n", argv[0]);
        exit(1);
    }
    if (strcmp(argv[1], "") == 0)
    {
        fprintf(stderr, "Sir %s vid\n", argv[1]);
        exit(1);
    }
    if (argv[1][strlen(argv[1]) - 1] == '/')
        argv[1][strlen(argv[1]) - 1] = '\0';
    delete_dir(argv[1]);

    return 0;
}