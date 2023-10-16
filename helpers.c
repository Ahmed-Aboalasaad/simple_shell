#include "main.h"

/**
 * print - prints message to the file that fd correesponds to
 *
 * @fd: the file descriptor
 * @message: the message
 * Return: nothing
*/
void print(int fd, char *message)
{
	ssize_t status;

	status = write(fd, message, _strlen(message));
	if (status == -1)
		perror("Writ() Error");
}

/**
 * slice - slices a string into a NULL-terminated array of strings based on
 * the given delimiter & doens't change the given string
 *
 * @str: the string to slice
 * @delimiter: the delimiter (slice at this character)
 * Return: a NULL-terminated array of sliced strings, or NULL if it fails
*/
char **slice(char *str, char *delimiter)
{
	int i = 0, sliceCount = 0;
	char **result, *slice, *source, inToken;

	/* Input Validation */
	if (!str || !str[0] || !delimiter || !delimiter[0])
		return (NULL);

	/* Build the slices array */
	source = copyStr(str);
	for (inToken = 0; source[i]; i++)
	{
		if (!inToken && !contains(delimiter, source[i]))
			sliceCount++, inToken = 1;
		if (inToken && contains(delimiter, source[i]))
			inToken = 0;
	}

	result = malloc(sizeof(*result) * (sliceCount + 1));
	if (!result)
	{
		free(source);
		return (NULL);
	}
	result[sliceCount] = NULL;

	/* Fill in the result array */
	slice = copyStr(strtok(source, delimiter));
	for (i = 0; i < sliceCount; slice = copyStr(strtok(NULL, delimiter)), i++)
		result[i] = slice;

	free(source);
	return (result);
}

/**
 * interruption - prompts the user again when they hit Ctrl+C (iterruption)
 *
 * @dummy: a dummy integer variable to suit the signal() syscall parameters
 * Return: nothing
*/
void interruption(int dummy)
{
	(void) dummy;
	print(STDOUT_FILENO, "\n#cisfun$ ");
	fflush(stdout);
}
