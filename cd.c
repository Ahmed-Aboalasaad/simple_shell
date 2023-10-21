#include "main.h"

/* Prototypes*/
void cd2(char *elcase, char *shellName, size_t *commandID, Command *command);
void cd(char *shellName, size_t *commandID, Command *command);
void cdError(char *shellName, size_t *commandID, Command *command);

/**
 * cd2 - cd continued
 * @directory: directory to navigate to
 * @shellName: the shell name
 * @commandID: #of this command in the current session
 * @command: the command itself
 *
 * Return: nothing
 */
void cd2(char *directory, char *shellName, size_t *commandID, Command *command)
{
    char *newPath = NULL, oldPath[1024];
    int i;
    struct stat status;

    if (directory) /* HOME or OLDPWD*/
    {
        char **directoryEnv = slice(_getenv(directory, &i), "=");
        newPath = directoryEnv[0];
        if (!newPath)
        {
            if (equal(directory, "OLDPWD"))
                newPath = "/"; /* not sure of this one */
            else
                cdError(shellName, commandID, command); /*here?*/
        }
    }
    else
        newPath = command->argv[1]; /* a directory path*/

    if (stat(newPath, &status) == 0)
    {
        if (getcwd(oldPath, sizeof(oldPath)))
        {
            _setenv("OLDPWD", oldPath);
            if (chdir(newPath) == 0)
            {
                print(STDOUT_FILENO, newPath); /*print the new directory*/
                print(STDOUT_FILENO, "\n");
                _setenv("PWD", newPath);
            }
            cdError(shellName, commandID, command);
        }
    }
    else
        cdError(shellName, commandID, command);
}

/**
 * cd -
 * @shellName: the shell name
 * @commandID: #of this command in the current session
 * @command: the command itself
 *
 * Return: nothing
 */
void cd(char *shellName, size_t *commandID, Command *command)
{
    if (!(command->argv[1])) /*HOME*/
        cd2("HOME", shellName, commandID, command);
    else if (equal(command->argv[1], "-")) /*OLDPWD*/
        cd2("OLDPWD", shellName, commandID, command);
    else /*A DIRECTORY*/
        cd2(NULL, shellName, commandID, command);
}

/**
 * cdError - prints error message when cd fails and frees the memory
 * @shellName: the shell name
 * @commandID: #of this command in the current session
 * @command: the command itself
 *
 * Return: nothing
 */
void cdError(char *shellName, size_t *commandID, Command *command)
{
    char *str;

    print(STDERR_FILENO, shellName);
    print(STDERR_FILENO, ": ");
    str = intToStr(*commandID);
    print(STDERR_FILENO, str);
    print(STDERR_FILENO, ": ");
    print(STDERR_FILENO, command->argv[0]);
    print(STDERR_FILENO, ": can't cd to ");
    print(STDERR_FILENO, command->argv[1]);
    print(STDERR_FILENO, "\n");
    free(str);
}
