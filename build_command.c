#include "main.h"

/**
 * buildCommand - finds out all the values of a command
 * (Reads its string, and Makes argv[] for it)
 *
 * @cmd: pointer to a command pointer
 * @str: the command line (if you have it). Set it to NULL if you don't
 * @commandID:pointer to the commandID variable in main()
 * it represents #commands done on this session
 *
 * Return:
 * 0 for success (It read the command, built it normally,
 * and it's ready to be executed),
 *
 * 1 if it read empty input("")
 * In htis case, you you should execute nothing & prompt again
*/
int buildCommand(Command **cmd, char *str, size_t *commandID)
{
	size_t commandSize;
	long charCount;
	Command *command;

	/* Allocation */
	(*commandID)++;
	*cmd = malloc(sizeof(**cmd));
	if (!*cmd)
		exit(EXIT_FAILURE);
	command = *cmd;
	command->str = NULL;
	command->argv = NULL;

	/* Fill the fields */
	if (str)
	{
		command->str = str, command->argv = slice(command->str, " ");
		return (0);
	}
	charCount = _getline(&command->str, &commandSize, STDIN_FILENO);
	if (charCount == -1) /* Reading error */
	{
		free(command);
		exit(EXIT_FAILURE);
	}
	if (charCount == 0) /* EOF without any chars before.. Close the shell */
	{
		free(command);
		print(STDOUT_FILENO, "\n");
		exit(EXIT_SUCCESS);
	}
	if (command->str[0] == '\n') /* Empty Command ("\n") */
	{
		free(command->str);
		free(command);
		return (1);
	}
	command->argv = slice(command->str, " \n");
	return (0);
}

/**
 * _getline - reads a line from stream to string
 * (including the new line if it was the delimiter) and updates size if needed
 *
 * @string: pointer to the string where it should write what it reads
 * @size: pointer to the size of string
 * @stream: the stream it should read from
 *
 * Return: #characters read if it succeeds, -1 for errors
*/
long _getline(char **string, size_t *size, int stream)
{
	char *buffer, *tmp;
	int charCount, bufferedBefore;

	if (*string)
		free(*string);
	*string = malloc(1), *string[0] = '\0', buffer = malloc(BUFFER_SIZE);

	for (bufferedBefore = 0; 1; bufferedBefore = 1)
	{
		charCount = read(stream, buffer, BUFFER_SIZE - 1);
		if (charCount < 1)
		{
			free(*string);
			*string = NULL;
			free(buffer);
			if (charCount == -1)
				return (-1); /* Reading Error or Signal Interruption */
			if (!bufferedBefore) /* reaching here means charCount == 0 */
				return (0); /* read EOF without any characters before */
		}
		buffer[charCount] = '\0';
		tmp = *string;
		*string = concatStr(*string, buffer);
		free(tmp);
		if (charCount < BUFFER_SIZE - 1)
			break;
	}
	free(buffer);
	*size = _strlen(*string);
	return (*size);
}
