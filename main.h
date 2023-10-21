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
#include "signal.h"
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
 * struct Script- a complete script of one or more commands
 * @str: all the script in one string
 * @commands: array of commands
 *
 */
typedef struct Script
{
	char *str;
	Command **commands;
} Script;

extern int previousExitValue;
extern char **env;
extern size_t commandID;
extern char *shellName;

/* Main */
long readLine(char **string, int stream);
Command *buildCommand(char *readyStr);
int buildScript(Script **s);
void executeCommand(Command *command, Script *script);
int getExitStatus(Command *command);
void cd(Command *command);

/* Helpers */
void print(int fd, char *message);
char **slice(char *str, char *delimiter);
void interruption(int dummy);

/* String Manipulators */
char *copyStr(char *source);
char *concatFile(char *folder, char *file);
char *intToStr(size_t x);
void reverseStr(char **s);
int equal(char *s1, char *s2);
int _strlen(const char *str);
char *concatStr(char *s1, char *s2);
int isPath(char *str);
char contains(const char *str, char c);
char isPositiveDigits(char *s);

/* Environment */
char *_getenv(const char *var, int *index);
int _setenv(const char *name, const char *value);
int _unsetenv(const char *name);

/* String Tokenizer */
char *_strtok(char *str, char *delimiter);

/* Memory Freeing*/
void freeCommand(Command *command);
void freeScript(Script *script);
#endif
