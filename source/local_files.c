#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <errno.h>
#include <stdarg.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/types.h>

#include "local_files.h"

#define localSharedPath "/JKSV2"

FILE *get_local_file_handle(char *relativePath, char *mode) {
    char *absoluteLocalPath = copy_absolute_local_path(relativePath);
    FILE *localFile = fopen(absoluteLocalPath, mode);
    free(absoluteLocalPath);
    return localFile;
}

DIR *get_local_dir_handle(char *dirPath) {
    char *absoluteLocalPath = copy_absolute_local_path(dirPath);
    printf("gldh: 26, %s\n", absoluteLocalPath);
    DIR *openedDir = opendir(absoluteLocalPath);
    printf("gldh: 28\n");
    free(absoluteLocalPath);
    return openedDir;
}

char *copy_concatenated_path(char *basePath, char *addedComponent) {
    int newPathLength;
    char *newPath;
    newPathLength = snprintf(NULL, 0, "%s/%s", basePath, addedComponent)+1;
    newPath = (char *)malloc(newPathLength+1);
    memset(newPath, 0, newPathLength+1);
    snprintf(newPath, newPathLength, "%s/%s", basePath, addedComponent);
    return newPath;
}

void local_path_stat(char *relativePath, struct stat *path_stat) {
    char *absolutePath = copy_absolute_local_path(relativePath);
    stat(absolutePath, path_stat);
    free(absolutePath);
}

char *copy_absolute_local_path(char *relativePath) { //if you use this, you better free it
    int absoluteLocalLength;
    char *absoluteLocalPath;
    absoluteLocalLength = snprintf(NULL, 0, "%s%s", localSharedPath, relativePath)+1;
    absoluteLocalPath = (char *)malloc(absoluteLocalLength+1);
    memset(absoluteLocalPath, 0, absoluteLocalLength+1);
    snprintf(absoluteLocalPath, absoluteLocalLength, "%s%s", localSharedPath, relativePath);
    return absoluteLocalPath;
}