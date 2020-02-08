#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include "queue.h"

int main(int argc, char* argv[])
{
    int opt;
    int L = 0, d = 0, g = 0, i =0, p = 0, s = 0, t = 0, u = 0, l = 0;
    struct dirent *dirInfo;
    struct queue* que;
    char directory[255];
    char* cwd = directory;
    char* next = (char*) malloc(255);
    char* simpleDirectory;

    while ((opt = getopt(argc, argv, "hLtpiugsdl")) != -1)
    {
        switch(opt)
        {
            case 'h':
                printf("Usage: %s [-h] [-L -d -g -i -p -s -t -u | -l] [dirname]\n", argv[0]);
                printf("        h - Print a help message and exit\n");
                printf("        L - Follow symbolic links\n");
                printf("        t - Print information on file type\n");
                printf("        p - Print permission bits as rwxrwxrwx\n");
                printf("        i - Print the number of links to file in inode table\n");
                printf("        u - Print the UID associated with the file\n");
                printf("        g - Print the GID associated with the file\n");
                printf("        s - Print the file size in bytes\n");
                printf("        d - Show the time of last modification\n");
                printf("        l - This option prints information on the file as if the options tpiugs are all specified\n");
                exit(EXIT_FAILURE);
            case 'L':
                L = 1;
                break;
            case 'd':
                d = 1;
                break;
            case 'g':
                g = 1;
                break;
            case 'i':
                i = 1;
                break;
            case 'p':
                p = 1;
                break;
            case 's':
                s = 1;
                break;
            case 't':
                t = 1;
                break;
            case 'u':
                u = 1;
                break;
            case 'l':
                t = p = i = u = g = s = 1;
                break;
            default:
                fprintf(stderr, "Usage: %s [-h] [-L -d -g -i -p -s -t -u | -l] [dirname]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if(argv[optind] != NULL)
    {
        chdir(argv[optind]);
    }

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
        cwd = dequeue(que);
        chdir(cwd);

        DIR *dir = opendir(cwd);
        while ((dirInfo = readdir(dir)) != NULL)
        {
            struct stat statbuf;

            if (strcmp(dirInfo->d_name, ".") != 0 && strcmp(dirInfo->d_name, "..") != 0)
            {
                stat(dirInfo->d_name, &statbuf);
                if (S_ISDIR(statbuf.st_mode) != 0)
                {
                    strcpy(next, cwd);
                    strcat(next, "/");
                    strcat(next, dirInfo->d_name);
                    enqueue(que, next);
                }

                if (t == 1)
                {
                    if (S_ISDIR(statbuf.st_mode) != 0)
                    {
                        printf("d");
                    }
                    else if (S_ISLNK(statbuf.st_mode) != 0)
                    {
                        printf("l");
                    }
                    else
                    {
                        printf("-");
                    }
                    if (p == 0)
                    {
                        printf(" ");
                    }
                }

                if (p == 1)
                {
                    if (S_IRUSR)
                    {
                        printf("r");
                    }
                    else
                    {
                        print("-");
                    }

                    if (S_IWUSR)
                    {
                        printf("w");
                    }
                    else
                    {
                        printf("-");
                    }

                    if (S_IXUSR)
                    {
                        printf("x");
                    }
                    else
                    {
                        printf("-");
                    }

                    if (S_IRGRP)
                    {
                        printf("r");
                    }
                    else
                    {
                        printf("-");
                    }

                    if (S_IWGRP)
                    {
                        printf("w");
                    }
                    else
                    {
                        printf("-");
                    }

                    if (S_IXGRP)
                    {
                        printf("x");
                    }
                    else
                    {
                        printf("-");
                    }

                    if (S_IROTH)
                    {
                        printf("r");
                    }
                    else
                    {
                        printf("-");
                    }

                    if (S_IWOTH)
                    {
                        printf("w");
                    }
                    else
                    {
                        printf("-");
                    }

                    if (S_IXOTH)
                    {
                        printf("x");
                    }
                    else
                    {
                        printf("-");
                    }
                    printf(" ");
                }

                if( i == 1)
                {
                    printf("%*i ", 3, statbuf.st_nlink);
                }

                if( u == 1)
                {
                    struct passwd *uid = getpwuid(statbuf.st_uid);
                    printf("%*s ", 8, uid->pw_name);
                }

                if( g == 1)
                {
                    struct group *gid = getgrgid(statbuf.st_gid);
                    printf("%*s ", 8, gid->gr_name);
                }

                printf("%s/%s\n", strstr(cwd, simpleDirectory), dirInfo->d_name);
            }
        }
        close(dir);
    }
    return 0;
}
