shell.c
Details
Activity
shell.c
Sharing Info
Not shared
General Info
Type
C
Size
4 KB (4,380 bytes)
Storage used
4 KB (4,380 bytes)
Location
New Project-20161024
Owner
me
Modified
Oct 24, 2016 by me
Opened
9:30 AM by me
Created
Oct 24, 2016 with Coding Ground
Description
Add a description
Download permissions
Viewers can download

Get notifications on your computer for shared files and important events.
TURN ON

New Team Drive

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

#include "header.h"

extern int read_sector(int sector_number, unsigned char* buffer);
extern int write_sector(int sector_number, unsigned char* buffer);

extern unsigned int get_fat_entry(int fat_entry_number, unsigned char* fat);
extern void set_fat_entry(int fat_entry_number, int value, unsigned char* fat);

// 13 is NOT the correct number -- you fix it!
#define BYTES_TO_READ_IN_BOOT_SECTOR 12

char* readInput();
char**parseInput(char*);
int run(char**);
int execute(char**);
int commandCount();
int exitShell(char**);

int cd(char**);
int pwd(char**);
int ls(char**);
int pbs(char**);
int pfe(char**);

char *commands[] =
{
    "exitShell",
    "cd",
    "pwd",
    "ls",
    "pbs",
    "pfe"
};
int (*commandFunctions[]) (char **) =
{
    &exitShell,
    &cd,
    &pwd,
    &ls,
    &pbs,
    &pfe
};

int main(int argc, char *argv[])
{
    char *input;
    char **args;
    int status = 0;

    do
    {
        printf("➜ ");
        input = readInput();
        args = parseInput(input);
        status = run(args);

        free(input);
        free(args);
    } while (status != -1);

    return EXIT_SUCCESS;
}

char *readInput()
{
    //buffer to store input, and buffer size setting
    char *line = NULL;
    ssize_t bufsize = 0;
    
    //note: getline dynamically allocates when given a null buffer
    getline(&line, &bufsize, stdin);
    return line;
}

char **parseInput(char *line)
{
    //buffer to allow up to 128 arguments, 
    int bufsize = 128, position = 0;
    char **buffer = malloc(128 * sizeof(char*));
    char *arg;

    // Check if buffer isn't allocated correctly.
    if (!buffer)
    {
        fprintf(stderr, "\"buffer\" double pointer character array did not allocate correctly in parseInput().\n");
        exit(EXIT_FAILURE);
    }

    // Every ' ' and '\n' is a delimiter to find each argument.
    arg = strtok(line, " \n");

    // For every actual argument, put it in buffer.
    while (arg != NULL)
    {
        buffer[position] = arg;
        position++;

        // Same as above, will continue from end of previous arg because of NULL argument
        arg = strtok(NULL, " ");
    }

    //remove the newline from the end of the last argument
    int i = 0;
    //find the terminator of the last argument
    while(buffer[position - 1][i] != '\0')
        i++;
    //check if the last character is a newline, and if it is, change it to NULL
    if (buffer[position - 1][i - 1] == '\n')
        buffer[position - 1][i - 1] = '\0';
    
    // Mark final position as NULL to mark the end of the buffer
    buffer[position] = NULL;
    return buffer;
}

int run(char **args)
{
  int i;

  if (args[0] == NULL)
  {
    // An empty command was entered.
    return 1;
  }

  for (i = 0; i < commandCount(); i++)
  {
      if (strcmp(args[0], commands[i]) == 0)
      {
          return (*commandFunctions[i])(args);
      }
  }

  return execute(args);
}

int execute(char **args)
{
    int i = 0;
    //forks the process so the child can run the command
    int isParent = fork();
    
    //parent waits
    if (isParent)
        wait(NULL);
    
    else if ( execvp(args[0], args) )
        printf( "Err: Unrecognized command\n" );
    
   return 0;
}

int commandCount() 
{
    return sizeof(commands) / sizeof(char *);
}

int exitShell(char** args)
{
    return -1;
}

int cd(char** args)
{
    //if there was an argument after cd then change directory
    if( args[1] )
    {
        //if an error occured
        if( chdir(args[1]) )
        {
            printf("An error occured, could not change directory \n");
            errno = 0;
        }
    }
    
    //if no directory was provided
    else
        printf("Err: No directory specified.\n");
    
    return 0;
}

int pwd(char** args)
{
    printf("Lol you thought this was implemented.\n");
}

int ls(char** args)
{
    //if there was one argument after ls then address it
    if( args[1] )
    {
        //if an error occured
        if( chdir(args[1]) )
        {
            printf("An error occured, could not change directory.\n");
            errno = 0;
        }
    }
    
    //if the number of arguments exceeded one
    else
        printf("Err: Too many arguments.\n");
    
    return 0;
}

int pbs(char** args)
{
    printf("Lol you thought this was implemented.\n");
}

int pfe(char** args)
{
    printf("Lol you thought this was implemented.\n");
}
