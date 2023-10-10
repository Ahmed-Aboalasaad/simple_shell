/*
 * This file has general helper functions that many function from
 * different files will use
 */
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
	size_t length;
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
	int i, j, sliceCount;
	char **result, *slice, *source;

	/* Input Validation */
	if (!str || !str[0] || !delimiter || !delimiter[0])
		return (NULL);

	/* Prepare the slices array */ // Error: counts the delimiters more than one time
	source = copyStr(str);
	for (i = 0, sliceCount = 1; source[i]; i++)
		for (j = 0; delimiter[j]; j++)
			if (source[i] == delimiter[j])
				sliceCount++;

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
 * Return: nothing
*/
void interruption()
{
	print(STDOUT_FILENO, "\n#cisfun$ ");
	fflush(stdout);
}
