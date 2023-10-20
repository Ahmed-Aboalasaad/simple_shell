#include "main.h"

/**
 * buildCommand - finds out all the values of a command
 * (Reads its string, and Makes argv[] for it)
 *
 * @readyStr: the command line (if you have it). Set it to NULL if you don't
 * @commandID:pointer to the commandID variable in main()
 * it represents #commands done in the running shell
 *
 * Return: a pointer to a newly constructed command structure
 * or NULL for errors
 */
Command *buildCommand(char *readyStr, size_t *commandID)
{
	int i;
	long charCount;
	Command *command;

	/* Allocation */
	(*commandID)++;
	command = malloc(sizeof(*command));
	if (!command)
		exit(EXIT_FAILURE);
	command->str = NULL;
	command->argv = NULL;

	/* Fill the fields */
	if (readyStr)
	{
		command->str = readyStr, command->argv = slice(readyStr, " ");
		return (command);
	}
	charCount = readLine(&command->str, STDIN_FILENO);
	if (charCount == -1) /* Reading error */
	{
		free(command);
		print(STDERR_FILENO, "Reading Error");
		exit(EXIT_FAILURE);
	}
	if (charCount == 0) /* EOF without any chars before.. Close the shell */
	{
		free(command);
		print(STDOUT_FILENO, "\n");
		for (i = 0; env[i]; i++)
			free(env[i]);
		free(env);
		exit(EXIT_SUCCESS);
	}
	command->argv = slice(command->str, " \n");
	return (command);
}

/**
 * readLine - reads a line from stream to string
 * (including the new line if it was the delimiter) and updates size if needed
 *
 * @string: pointer to the string where it should write what it reads
 * @stream: the stream it should read from
 *
 * Return: #characters read if it succeeds, -1 for errors
 */
long readLine(char **string, int stream)
{
	char *buffer, *tmp;
	int charCount, bufferedBefore;

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
		fflush(stdout);
		*string = concatStr(*string, buffer);
		free(tmp);
		if (charCount < BUFFER_SIZE - 1)
			break;
	}
	free(buffer);
	return (_strlen(*string));
}
