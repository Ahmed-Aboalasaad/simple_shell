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

	for (length = 0; message[length];)
		length++;
	status = write(fd, message, length);
	if (status == -1)
		perror("Writ() Error");
}

/**
 * slice - slices a string into a NULL-terminated array of strings based on
 * the given delimiter & doens't change the given string
 *
 * @str: the string to slice
 * @delimiter: the delimiter (slice at this character)
 * (This version of slice accepts a delimiters string of only one character)
 * Return: a NULL-terminated array of sliced strings, or NULL if it fails
*/
char **slice(char *str, char *delimiter)
{
	int i, sliceCount;
	char **result, *slice, *source;

	/* Input Validation */
	if (!str || !str[0] || !delimiter || !delimiter[0])
		return (NULL);

	/* Prepare the slices array */
	source = copyStr(str);
	for (i = 0, sliceCount = 1; source[i]; i++)
		if (source[i] == delimiter[0])
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
