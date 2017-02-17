// Methods to manipulate remote files over SFTP with libssh2
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <errno.h>
#include <stdarg.h>
#include <unistd.h>
#include <dirent.h>

#include <fcntl.h>

#include <sys/types.h>

#include <3ds/console.h>

#include "remote_files.h"
#include "local_files.h"

void push_local_file(char *dirPath) {
    FILE *localFile = get_local_file_handle(dirPath, "rb");
    char *fullRemotePath;
    struct stat fileInfo;
    local_path_stat(dirPath, &fileInfo);
    printf("%s\n->%llu\n", dirPath, (intmax_t)fileInfo.st_size);
    // fflush(stdout);
    fclose(localFile);
}

void push_local_directory(char *dirPath) {
    char absolutePath[512];
    snprintf(absolutePath, sizeof(absolutePath), "%s%s", "/JKSV2", dirPath);
    printf("%s\n", dirPath);
    DIR *parentDirectory = opendir(absolutePath);
    printf("-> opened\n");
    struct dirent *dir = NULL;
    if (parentDirectory)
    {
        while ((dir = readdir(parentDirectory)) != NULL)
        {
            char filePath[512]; 
            snprintf(filePath, sizeof(filePath), "%s/%s", dirPath, dir->d_name);
            struct stat path_stat;
            local_path_stat(filePath, &path_stat);
            if (S_ISREG(path_stat.st_mode)) {
                push_local_file(filePath);
            } else if (S_ISDIR(path_stat.st_mode)) {
                push_local_directory(filePath);
            }
        }
        closedir(parentDirectory);
        //free(prev_dir);
    }
}