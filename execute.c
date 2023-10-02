#include "main.h"

void executeByPath(Command *command);
int executeBuiltIns(char **argv);
int setPath(char **argv);

void notFound(char *shellName, size_t *commandCount, Command *command,
char commandCountStr)
{
	int i;

	print(STDERR_FILENO, shellName);
	print(STDERR_FILENO, ": ");
	commandCountStr = intToStr(*commandCount);
	print(STDERR_FILENO, commandCountStr);
	print(STDERR_FILENO, ": ");
	print(STDERR_FILENO, command->argv[0]);
	print(STDERR_FILENO, ": not found\n");
	for (i = 0; command->argv[i]; i++)
		free(command->argv[i]);
	free(command->argv);
	free(command->str);
	free(commandCountStr);
	free(command);
}

/**
 * executeCommand - executes a command and then frees its feilds
 *
 * @command: The command struct
 * @shellName: the name of the running shell
 * @commandCount: the number of the command being builded right now
 * @interactive: interactive ?
 * Return: void
*/
void executeCommand(Command *command, char *shellName, size_t *commandCount
, int interactive)
{
	pid_t id;
	int i, status, result;
	char *commandCountStr;

	result = executeBuiltIns(command->argv);
	if (result > -1) /* a built-in was found */
	{
		free(command->str);
		for (i = 0; command->argv[i]; i++)
			free(command->argv[i]);
		free(command->argv);
		free(command);
		if (result)
			exit(EXIT_SUCCESS);
		return;
	}
	if (setPath(command->argv) != 0) /* no such program exists */
	{
		notFound(shellName, commandCount, command, commandCountStr);
		return;
	}
	id = fork();
	if (id) /* Parent */
	{
		wait(&status);
		if (interactive)
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

	/* Don't search for an executable if the input is path already */
	if (isPath(argv[0]))
		return (stat(argv[0], &status) != 0); /* stat() returns 0 for success */

	/* Search for the executable file */
	paths = slice(_getenv("PATH", &i), ":");
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
 * 0 if a built in was found was found
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
	 *	_cd(argv[1]);
	 *	return (0);
	 * }
	*/
	return (-1);
}
