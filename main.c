#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include "queue.h"

int main(void)
{
    struct dirent *dirInfo;
    struct queue* que;
    char directory[255];
    char* cwd = directory;
    char* next = (char*) malloc(255);
    char* simpleDirectory;

    getcwd(directory, sizeof(directory));

    if (strcmp(directory, "/") != 0)
    {
        simpleDirectory = strrchr(cwd, '/');
        simpleDirectory++;
    }

    que = malloc(sizeof(struct queue));
    createQueue(que);
    enqueue(que, cwd);

    while (que->first != NULL)
    {
        next = dequeue(que);

        if(strcmp(cwd, next) != 0)
        {


            DIR *dir = opendir(cwd);
            while ((dirInfo = readdir(dir)) != NULL)
            {
                struct stat statbuf;

                if (strcmp(dirInfo->d_name, ".") != 0 && strcmp(dirInfo-                                   >d_name, "..") != 0)
                {
                    stat(dirInfo->d_name, &statbuf);
                    if (S_ISDIR(statbuf.st_mode) != 0)
                    {
                        //strcpy(next, simpleDirectory);
                        //strcat(next, "/");
                        //strcat(next, dirInfo->d_name);
                        //printf("is dir");
                        enqueue(que, dirInfo->d_name);
                    }
                    printf("%s/%s\n", simpleDirectory, dirInfo->d_na                                   me);
                }
            }
            close(dir);
        }
        return 0;
    }
