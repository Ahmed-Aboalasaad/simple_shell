#include "main.h"

void notFound(char *shellName, size_t *commandID, Command *command);
void executeByPath(Command *command);
int executeBuiltIns(Command *command, char *shellName,
								size_t *commandID, Script *script);
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
					size_t *commandID, Script *script)
{
	int IsBuiltIn, childExitStatus;

	IsBuiltIn = executeBuiltIns(command,shellName, commandID, script);
	if (IsBuiltIn) /* a built-in was found */
		return;

	if (setPath(command->argv)) /* no such accessible program exists */
	{
		notFound(shellName, commandID, command);
		return;
	}
	if (fork()) /* Parent */
	{
		wait(&childExitStatus);
		previousExitValue = WIFEXITED(childExitStatus)? WEXITSTATUS(childExitStatus) : 0;
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
	char *str;

	print(STDERR_FILENO, shellName);
	print(STDERR_FILENO, ": ");
	str = intToStr(*commandID);
	print(STDERR_FILENO, str);
	print(STDERR_FILENO, ": ");
	print(STDERR_FILENO, command->argv[0]);
	print(STDERR_FILENO, ": not found\n");
	free(str);
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
	execve(command->argv[0], command->argv, __environ);

	/* Contunuing after execve() means an error occurred */
	freeCommand(command);
	exit(EXIT_FAILURE); /* what about script freeing?*/
}

/**
 * executeBuiltIns - checks the argv, and executes the required
 * built-in (if any)
 *
 * @argv: the argument vecotr
 * Return:
 * 0 if no built in commands were found
 * 1 if a built in command was found
 */
int executeBuiltIns(Command *command, char *shellName,
					size_t *commandID, Script *script)
{
	if (equal(command->argv[0], "exit"))
	{
		int exitStatus;

		exitStatus = getExitStatus(shellName, commandID, command);
		if (exitStatus == -1) /* invalid exit status */
			return (1); /* and the script/command will be freed in main() */
		if (script)
			freeScript(script);
		else
			freeCommand(command);
		exit(exitStatus);
	}
	else if (equal(command->argv[0], "setenv"))
	{
		_setenv(command->argv[1], command->argv[2], equal(command->argv[3], "1"));
		return (1);
	}
	else if (equal(command->argv[0], "unsetenv"))
	{
		_unsetenv(command->argv[1]);
		return (1);
	}
	return (0);
}
