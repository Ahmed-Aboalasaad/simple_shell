#include "main.h"

/**
 * copyStr - copies a string into another one
 *
 * @source: the string to be copied
 * Return: a pointer to the newly made string
*/
char *copyStr(char *source)
{
	int i, length;
	char *result;

	if (!source)
		return (NULL);

	for (i = length = 0; source[i]; i++)
		length++;
	result = malloc(sizeof(*result) * (length + 1));
	if (!result)
		return (NULL);
	result[length] = '\0';
	for (i = 0; i < length; i++)
		result[i] = source[i];
	return (result);
}

/**
 * concatFile - concatenates a folder & a file names and puts a '/'
 * between them
 *
 * @folder: the first string
 * @file: the seoncd string
 * Return: pointer to the concatenated string or NULL if it fails
 */
char *concatFile(char *folder, char *file)
{
	size_t i, len1, len2;
	char *result;

	/* Input Validation */
	if (!folder || !file)
		return (NULL);

	for (len1 = 0; folder[len1];)
		len1++;
	for (len2 = 0; file[len2];)
		len2++;
	result = malloc(sizeof(*result) * (len1 + len2 + 2));
	if (!result)
		return (NULL);
	result[len1 + len2 + 1] = '\0';
	for (i = 0; i < len1; i++)
		result[i] = folder[i];
	result[i] = '/';
	for (i = 0; i < len2; i++)
		result[i + len1 + 1] = file[i];
	return (result);
}

/**
 * intToStr - converts a non-negative integer to a string
 *
 * @x: the integer
 * Return: a pointer to the converted string, or NULL in case of failure
*/
char *intToStr(size_t x)
{
	size_t i, xCopy;
	int digitCount;
	char *reversed;

	for (digitCount = 0, xCopy = x; xCopy > 0; xCopy /= 10)
		digitCount++;

	reversed = malloc(sizeof(*reversed) * digitCount + 1);
	if (!reversed)
		return (NULL);
	reversed[digitCount] = '\0';

	for (i = 0, xCopy = x; xCopy > 0; i++, xCopy /= 10)
		reversed[i] = (xCopy % 10) + '0';

	reverseStr(&reversed);
	return (reversed);
}

/**
 * reverseStr - reverses a string
 *
 * @s: the string to be reversed
 * Return: the reversed string
*/
void reverseStr(char **s)
{
	int i, length;
	char *reversed;

	if (!(*s))
		return;

	length = _strlen(*s);
	reversed = malloc(sizeof(*reversed) * (length + 1));
	if (!reversed)
		return;
	reversed[length] = '\0';

	for (i = 0; i < length; i++)
		reversed[i] = (*s)[length - i - 1];
	free(*s);
	*s = reversed;
}

/**
 * equal - check if the given string are identical
 *
 * @s1: the first string
 * @s2: the second string
 * Return: 1 if they're identical, 0 otherwise
*/
int equal(char *s1, char *s2)
{
	int i, len1, len2;

	if (!s1 || !s2)
		return (0);

	/* Check Lengths */
	for (i = len1 = 0; s1[i]; i++)
		len1++;
	for (i = len2 = 0; s2[i]; i++)
		len2++;
	if (len2 != len1)
		return (0);

	/* Check Values */
	for (i = 0; i < len1; i++)
		if (s1[i] != s2[i])
			return (0);

	/* passed the tests */
	return (1);
}
