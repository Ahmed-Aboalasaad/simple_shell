#include "main.h"

void cd(Command *command);
void cdError(Command *command);

/**
 * cd2 - cd continued
 * @envVar: directory to navigate to
 * @command: the command itself
 *
 * Return: nothing
 */
void cd2(char *envVar, Command *command)
{
	char *destination = NULL, currentDirectory[1024];
	int i;
	struct stat status;

	if (envVar) /* either go HOME or OLDPWD */
	{
		destination = _getenv(envVar, &i);
		if (!destination) /* no such environment variable */
			cdError(command);
	}
	else /* a path is given in the command */
		destination = command->argv[1];

	if (stat(destination, &status)) /* there isn't such a destination */
	{
		cdError(command);
		return;
	}

	if (getcwd(currentDirectory, sizeof(currentDirectory)))
	{
		if (chdir(destination) == 0) /* changed directory successfully */
		{
			_setenv("OLDPWD", currentDirectory);
			if (equal(command->argv[1], "-"))
			{
				print(STDOUT_FILENO, destination);
				print(STDOUT_FILENO, "\n");
			}
			_setenv("PWD", destination);
			return;
		}
		cdError(command);
	}
	else
		cdError(command);
}

/**
 * cd - change directory built in
 * @command: the command itself
 *
 * Return: nothing
 */
void cd(Command *command)
{
	if (!(command->argv[1])) /* go HOME */
		cd2("HOME", command);
	else if (equal(command->argv[1], "-")) /* go OLDPWD */
		cd2("OLDPWD", command);
	else /* go to the given directory */
		cd2(NULL, command);
}

/**
 * cdError - prints error message when cd fails and frees the memory
 * @command: the command itself
 *
 * Return: nothing
 */
void cdError(Command *command)
{
	char *str;

	print(STDERR_FILENO, shellName);
	print(STDERR_FILENO, ": ");
	str = intToStr(commandID);
	print(STDERR_FILENO, str);
	print(STDERR_FILENO, ": ");
	print(STDERR_FILENO, command->argv[0]);
	print(STDERR_FILENO, ": can't cd to ");
	print(STDERR_FILENO, command->argv[1]);
	print(STDERR_FILENO, "\n");
	free(str);
}
