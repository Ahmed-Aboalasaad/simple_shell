#include "main.h"

void illegalNumber(char *shellName, size_t *commandID, Command *command);

/**
 * getExitStatus - exits the shell with the given status in command
 * (if it's valid)
 *
 * @shellName: the shell name
 * @commandID: #commands done in the running shell
 * @command: the exit command (having the status)
 *
 * Return: exit status if it's valid or -1 if it's invalid
 */
int getExitStatus(char *shellName, size_t *commandID, Command *command)
{
	ssize_t intStatus, i;
	char *strStatus = command->argv[1];

	if (!strStatus) /* exit command without status */
		return (0);
	if (!strStatus[0]) /* command->argv[0] is an empty string */
		exit(4);

	if (!isPositiveDigits(strStatus)) /* letters or negative number */
	{
		illegalNumber(shellName, commandID, command);
		return (-1);
	}

	/* the string is positive digits.. turn it into a number */
	for (i = intStatus = 0; strStatus[i]; i++)
	{
		intStatus *= 10;
		intStatus += (strStatus[i] - '0');
	}
	if (intStatus > 255)
	intStatus = intStatus % 256;
	return (intStatus);
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
