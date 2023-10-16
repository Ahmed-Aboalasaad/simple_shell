#include "main.h"

/* Prototypes */
int exitHandler(char *shellName, size_t *commandID, Command *command);
void illegalNumber(char *shellName, size_t *commandID, Command *command);

/**
 * exitHandler - Handles exit command, if a status exists
 * @shellName: the shell name
 * @commandID: # of this command in the current session
 * @command: the command itself
 *
 * Return: exit status or -1 if invalid
 */
int exitHandler(char *shellName, size_t *commandID, Command *command)
{
	ssize_t num = 0, i = 0;
	char *exitStatus = command->argv[1];

	if (exitStatus)
	{
		for (i = 0; exitStatus[i]; i++)
		{
			if ((exitStatus[i] >= 48) && (exitStatus[i] <= 57))
			{
				num *= 10;
				num += (exitStatus[i] - '0');
			}
			else
			{
				illegalNumber(shellName, commandID, command);
				num = -1;
				break;
			}
		}
		if (num > 255)
		num = num % 256;
	}
	return (num);
}

/**
 * illegalNumber - prints error message when an invalid exit code is provided
 * @shellName: the shell name
 * @commandID: #of this command in the current session
 * @command: the command itself
 *
 * Return: nothing
 */
void illegalNumber(char *shellName, size_t *commandID, Command *command)
{
	char *str;

	print(STDERR_FILENO, shellName);
	print(STDERR_FILENO, ": ");
	str = intToStr(*commandID);
	print(STDERR_FILENO, str);
	print(STDERR_FILENO, ": ");
	print(STDERR_FILENO, command->argv[0]);
	print(STDERR_FILENO, ": Illegal number: ");
	print(STDERR_FILENO, command->argv[1]);
	print(STDERR_FILENO, "\n");
	free(str);
}
