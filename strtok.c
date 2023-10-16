#include "main.h"
#include <stdlib.h>

/**
 * _strtok - splits a given string into tokens separated by given delimiter
 * @str: pointer to the string to be tokenized
 * @delimiter: pointer to string of delimiter characters
 *
 * Return: pointer to a string of the first token
 * or Null if there are no tokens
 */
char *_strtok(char *str, char *delimiter)
{
	char *token;
	static char *remaining;

	if (str)
	remaining = str;

	if (!remaining)
		return (NULL);

	while (*remaining && contains(delimiter, *remaining))
		remaining++;

	if (!*remaining)
		return (NULL);

	token = remaining;

	while (*remaining && !contains(delimiter, *remaining))
		remaining++;

	if (*remaining)
	{
		*remaining = '\0';
		remaining++;
	}
	else
		remaining = NULL;
	return (token);
}
