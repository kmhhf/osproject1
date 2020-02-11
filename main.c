//Kyle Moore
//02/03/2020
//This program will list all files in a directory and sub directories using a breadth first search.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
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
    DIR *dir;

    //Handles all of the options by setting variables to 1 if the option is used.
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
                printf("        l - This option prints information on the file as if the options tpiugs are a                                                                                                                ll specified\n");
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
                t = p = i = u = g = s = d = 1;
                break;
            default:
                fprintf(stderr, "Usage: %s [-h] [-L -d -g -i -p -s -t -u | -l] [dirname]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    //checks to see if a command line arguement was given and uses the arguement as the starting directory
    if(argv[optind] != NULL)
    {
        if(chdir(argv[optind]) != 0)  //error check for chdir
        {
            fprintf(stderr, "%s: Error: ", argv[0]);
            perror("");
            exit(EXIT_FAILURE);
        }
    }

    //gets the current directory and checks for errors
    if(getcwd(directory, sizeof(directory)) == NULL)
    {
        fprintf(stderr, "%s: Error: ", argv[0]);
        perror("");
        exit(EXIT_FAILURE);
    }

    //if the directory isnt root, sets the starting directory to a variable to use in the display of the results
    if (strcmp(directory, "/") != 0)
    {
        simpleDirectory = strrchr(cwd, '/');
        simpleDirectory++;
    }

    que = malloc(sizeof(struct queue));

    if(que == NULL)
    {
        fprintf(stderr, "%s: Error: ", argv[0]);
        perror("");
        exit(EXIT_FAILURE);
    }

    createQueue(que);               //creates the queue to hold the directories that needed to be checked
    enqueue(que, cwd);              //puts the starting directory in the queue

    while (que->first != NULL)      //runs as long as the queue is not empty
    {
        cwd = dequeue(que);     //gets next directory from the que
        if(chdir(cwd) != 0)     //changes current working directory to the next one in the que
        {
            fprintf(stderr, "%s: Error: ", argv[0]);
            perror("");
            exit(EXIT_FAILURE);
        }

        dir = opendir(cwd);     //opens the next directry and error check
        if(dir == NULL)
        {
            fprintf(stderr, "%s: Error: ", argv[0]);
            perror("");
            exit(EXIT_FAILURE);
        }

        while ((dirInfo = readdir(dir)) != NULL)   //goes through each file in the directory
        {
            struct stat statbuf;

            if (strcmp(dirInfo->d_name, ".") != 0 && strcmp(dirInfo->d_name, "..") != 0)  //removes . and .. from                                                                                                                 the results
            {
                //uses stat if the L option is set in order to follow soft links
                if ( L == 1 )
                {
                    if(stat(dirInfo->d_name, &statbuf) != 0)  //get file info
                    {
                        fprintf(stderr, "%s: Error: ", argv[0]);
                        perror("");
                        exit(EXIT_FAILURE);
                    }
                }
                else
                {
                    if(lstat(dirInfo->d_name, &statbuf) != 0)  //get file info without following links
                    {
                        fprintf(stderr, "%s: Error: ", argv[0]);
                        perror("");
                        exit(EXIT_FAILURE);
                    }
                }

                //check for direcories and add them to the queue
                if (S_ISDIR(statbuf.st_mode) != 0)
                {
                    strcpy(next, cwd);
                    strcat(next, "/");
                    strcat(next, dirInfo->d_name);
                    enqueue(que, next);
                }

                //if t option was used, check file permissions and prints them
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

                //if i option was used, checks the number of links and prints the result
                if( i == 1)
                {
                    printf("%*i ", 3, statbuf.st_nlink);
                }

                //if u option was used prints out the UID
                if( u == 1)
                {
                    struct passwd *uid = getpwuid(statbuf.st_uid);
                    printf("%*s ", 8, uid->pw_name);
                }

                //if g option was used prints out the GID
                if( g == 1)
                {
                    struct group *gid = getgrgid(statbuf.st_gid);
                    printf("%*s ", 8, gid->gr_name);
                }

                //if s option was used prints out the size of the file
                if( s == 1)
                {
                    long long size = statbuf.st_size;
                    if (size >= 1073741824)
                    {
                        printf("%*iG ", 5, size/1073741824);
                    }
                    else if (size >= 1048576)
                    {
                        printf("%*iM ", 5, size/1048576);
                    }
                    else if (size >= 1024)
                    {
                        printf("%*iK ", 5, size/1024);
                    }
                    else
                    {
                        printf("%*i ", 6, size);
                    }
                }

                //if d option was used prints out the last time the file was modified
                if( d == 1)
                {
                    char time[40];
                    strftime(time, 40, "%b %d, %Y", localtime(&statbuf.st_mtime));
                    printf("%*s ", 10, time);
                }

                printf("%s/%s\n", strstr(cwd, simpleDirectory), dirInfo->d_name); //print name and relative p                                                                                                                ath of file
            }
        }
        close(dir);
    }
    return 0;
}