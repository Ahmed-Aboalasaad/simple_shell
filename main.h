#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>
#define BUFFER_SIZE 1024

/**
 * struct Command - a command line
 * @str: the command line itself in a string
 * @argv: the argument vector (slicing the str)
 *
 * Description: holds the needed data for a command to get executed
 */
typedef struct Command
{
	char *str;
	char **argv;
} Command;

/**
 * struct Scr ipt- a complete script of one or more commands
 * @str: all the script in one string
 * @commands: array of commands
 *
 */
typedef struct Script
{
	char *str;
	Command **commands;
} Script;

int isVarName(const char *name, const char *str);

/* Main */
long _getline(char **string, size_t *n, int stream);
int buildCommand(Command **cmd, char *line, size_t *commandCount);
int buildScript(Script **s, size_t *commandCount);
void executeCommand(Command *command, char *shellName,
					size_t *commandID, int interactive);

/* Helpers */
void print(int fd, char *message);
char **slice(char *str, char *delimiter);

/* String Manipulators */
char *copyStr(char *source);
char *concatFile(char *folder, char *file);
char *intToStr(size_t x);
void reverseStr(char **s);
int equal(char *s1, char *s2);
int _strlen(const char *str);
char *concatStr(char *s1, char *s2);
int isPath(char *str);
char contains(char *str, char c);

/* Environment */
char *_getenv(const char *var, int *index);
int _setenv(const char *name, const char *value, int overwrite);
int _unsetenv(const char *name);

#endif
