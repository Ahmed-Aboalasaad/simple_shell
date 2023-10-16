#include "main.h"

/* Prototypes */
void notFound(char *shellName, size_t *commandID, Command *command);
void executeByPath(Command *command);
int executeBuiltIns(char **argv);
int setPath(char **argv);

/**
 * executeCommand - executes a command and then frees its feilds
 *
 * @command: The command struct
 * @shellName: the name of the running shell
 * @commandID: the number of the command being builded right now
 * @interactive: interactive ?
 * Return: void
 */
void executeCommand(Command *command, char *shellName,
                                        size_t *commandID, int interactive)
{
        int i, result, exitStatus;

        result = executeBuiltIns(command->argv);
        if (result) /* 1: exit */ 
                exitStatus = exitHandler(shellName, commandID, command);
        if (result > -1) /* a built-in was found 1:exit, 0:another built-in */
        {
                free(command->str);
                for (i = 0; command->argv[i]; i++)
                        free(command->argv[i]);
                free(command->argv);
                free(command);
                if (exitStatus > -1)
                        exit (exitStatus);
                return;
        }
        if (setPath(command->argv)) /* no such accessible program exists */
        {
                notFound(shellName, commandID, command);
                return;
        }
        if (fork()) /* Parent */
        {
                wait(NULL);
                if (interactive) // look here
                        free(command->str);
                for (i = 0; command->argv[i]; i++)
                        free(command->argv[i]);
                free(command->argv);
                free(command);
        }
        else /* Child */
                executeByPath(command);
}

/**
 * notFound - handle the case of an unknown command was input
 *
 * @shellName: the shell name
 * @commandID: #of this command in the current session
 * @command: the command itself
 * Return: nothing
 */
void notFound(char *shellName, size_t *commandID, Command *command)
{
        int i;
        char *str;

        print(STDERR_FILENO, shellName);
        print(STDERR_FILENO, ": ");
        str = intToStr(*commandID);
        print(STDERR_FILENO, str);
        print(STDERR_FILENO, ": ");
        print(STDERR_FILENO, command->argv[0]);
        print(STDERR_FILENO, ": not found\n");
        for (i = 0; command->argv[i]; i++)
                free(command->argv[i]);
        free(command->argv);
        free(command->str);
        free(str);
        free(command);
}

/**
 * setPath - searches for the "program" executable in the paths listed in
 * the PATH environment varaible. If it was found, it sets its (the first
 * element in argv) to the path
 *
 * @argv: the argument vector to be given for the program to be executed
 * Return: 0 for success (there is a program with such a name),
 * 1 otherwise (no such programs in the system or invalid input)
 */
int setPath(char **argv)
{
        int i;
        struct stat status;
        char **paths, *path;

        /* Input Validation */
        if (!argv || !argv[0])
                return (1);

        /* Don't search for an executable if the input is already a path */
        if (isPath(argv[0]))
                return (stat(argv[0], &status));

        /* Search for the executable file */
        paths = slice(_getenv("PATH", &i), ":");
        if (!paths)
                exit(EXIT_FAILURE);

        for (i = 0; paths[i]; i++)
        {
                path = concatFile(paths[i], argv[0]);
                if (stat(path, &status) == 0) /* stat() returns 0 for success */
                {
                        free(argv[0]);
                        argv[0] = copyStr(path);
                        for (i = 0; paths[i]; i++)
                                free(paths[i]);
                        free(paths);
                        free(path);
                        return (0);
                }
                free(path);
        }
        for (i = 0; paths[i]; i++)
                free(paths[i]);
        free(paths);
        return (1);
}

/**
 * executeByPath - executes a program located in the first string in the
 * given argv varaible
 *
 * @command: the command to be executed
 * Return: nothing
 */
void executeByPath(Command *command)
{
        int i;

        execve(command->argv[0], command->argv, __environ);

        /* Contunuing after execve() means an error occurred */
        free(command->str);
        for (i = 0; command->argv[i]; i++)
                free(command->argv[i]);
        free(command->argv);
        free(command);
        exit(EXIT_FAILURE);
}

/**
 * executeBuiltIns - checks tha argv, and executes the required
 * built-in (if any)
 *
 * @argv: the argument vecotr
 * Return:
 * 0 if a non-exit built in was found
 * 1 if an exit built in was found
 * -1 if no built-in was found
 */
int executeBuiltIns(char **argv)
{
        /* Check for built-Ins*/
        if (equal(argv[0], "exit"))
        {
                return (1);
        }
        else if (equal(argv[0], "setenv"))
        {
                _setenv(argv[1], argv[2], equal(argv[3], "1"));
                return (0);
        }
        else if (equal(argv[0], "unsetenv"))
        {
                _unsetenv(argv[1]);
                return (0);
        }
        /*
         * else if (equal(argv[0], "cd"))
         * {
         *      _cd(argv[1]);
         *      return (0);
         * }
         */
        return (-1);
}
