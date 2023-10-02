#include "main.h"

/**
 * _strlen - calculates the length of given string
 *
 * @str: the string to calculate its length
 * Return: int newValue represents length of string
 */
int _strlen(const char *str)
{
	int len;

	if (!str || !str[0])
		return (0);

	for (len = 0; str[len]; )
		len++;
	return (len);
}

/**
 * concatStr - concatenate two given strings
 *
 * @s1: first string
 * @s2: second string
 * Return: the concatenated string
 */
char *concatStr(char *s1, char *s2)
{
	int i, len1, len2;
	char *result;

	for (len1 = 0; s1[len1]; )
		len1++;
	for (len2 = 0; s2[len2]; )
		len2++;

	result = malloc(sizeof(*result) * (len1 + len2 + 1));
	if (!result)
		return (NULL);
	result[len1 + len2] = '\0';

	for (i = 0; i < len1; i++)
		result[i] = s1[i];
	for (i = 0; i < len2; i++)
		result[i + len1] = s2[i];

	return (result);
}

/**
 * isPath - check if the given string is a path (absolute/relative)
 *
 * @str: the string to be checked
 * Return: 1 if it's a path, 0 otherwise
*/
int isPath(char *str)
{
	int i;

	for (i = 0; str[i]; i++)
		if (str[i] == '/')
			return (1);
	return (0);
}
