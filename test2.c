#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>
#define BUFFER_SIZE 1024

char *buildEnvStr(const char *name, const char *value);
int isVarName(const char *prefix, const char *str);

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
 * _strlen - calculates the length of the given string
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
 * _getenv - gets the value of an environment variable named var
 *
 * @var: the environment variable name
 * @index: int pointer to write the index of the env var (if any) to
 * Return: pointer on the variable value
 * or NULL if there is no such variable
*/
char *_getenv(const char *name, int *index)
{
	int i, j;

	for (i = 0; __environ[i]; i++)
		if (isVarName(name, __environ[i]))
		{
			*index = i;
			for (j = 0; __environ[i][j] != '=';)
				j++;
			return (&__environ[i][j + 1]);
		}
	return (NULL);
}

/**
 * _setenv - adds/modifies an environment variable
 *
 * @name: variable name
 * @value: variable value
 * @overwrite: modify the variable if it exists ?
 * Return: 0 on success, 1 for errors
*/
int _setenv(char **envp, const char *name, const char *value, int overwrite)
{
	char *oldValue,*newString, **newEnviron;
	int i, varCount, index;

	newString = buildEnvStr(name, value), oldValue = _getenv(name, &index);
	if (!newString)
		return (1);
    printf("name: %s\n", name);
    printf("value: %s\n", value);
    printf("old value: %s\n", oldValue);
	if (oldValue) /* There is an environment variable with this name */
	{
        printf("There is a var with this name :)\n");
		if (!overwrite)
		{
            printf("before freeing\n");
			free(newString);
            printf("after freeing\n");
			return (0);
		}
		/* update __environ[] */
		/* substitue the old "var=value" string with the new one */
        printf("before freeing\n");
		free(envp[index]);
        printf("after freeing\n");
		envp[index] = newString;
		return (0);
	}
	
	/* Adding a new Variable */
	/* Build & Fill a new __environ[] array*/
	for (varCount = 0; envp[varCount]; )
		varCount++;
	newEnviron = malloc(sizeof(*newEnviron) * (varCount + 2));
	if (!newEnviron)
	{
		free(newString);
		return (1);
	}
	newEnviron[varCount + 1] = NULL;
	for (i = 0; i < varCount; i++)
		newEnviron[i] = envp[i];
	newEnviron[i] = newString;
	/* Substitue the old environ with the new one */
	for (i = 0; envp[i]; i++)
		free(envp[i]);
	envp = newEnviron;
	return (0);
}

/**
 * buildVarStr - builds a "var=value" string ready to be added to __environ[]
 *
 * @name: the variable name
 * @value: the variable value
 * Return: the new string if the input is valid & it succeeds, NULL otherwise
*/
char *buildEnvStr(const char *name, const char *value)
{
	int i;
	char *result;

	/* Input Validation */
	if (!name || !value)
		return (NULL);

	result = malloc(_strlen(name) + _strlen(value) + 2);
	if (!result)
		return (NULL);
	result[_strlen(name) + _strlen(value) + 1] = '\0';

	for (i = 0; name[i]; i++)
		result[i] = name[i];
	result[i] = '=';
	for (i = 0; value[i]; i++)
		result[i + _strlen(name) + 1] = value[i];

	return (result);
}

/**
 * _unsetenv - delete certain environment variable
 *
 * @name: variable name
 * Return: 0 on success, 1 for errors
*/
int _unsetenv(const char *name)
{
	char **newEnviron, *oldValue;
	int i, j, index, varNum;

	oldValue = _getenv(name, &index);
	if (!oldValue)
		return (0);

	/* Build a new environ[] array */
	for (varNum = 0; __environ[varNum]; )
		varNum++;
	newEnviron = malloc(sizeof(*newEnviron) * varNum);
	if (!newEnviron)
		return (1);
	newEnviron[varNum - 1] = NULL;

	/* Fill it */
	for (i = j = 0; i < varNum; i++)
		if (j != index)
			newEnviron[i] = copyStr(__environ[j++]);

	/* Substitute the old __environ with the new one */
	for (i = 0; __environ[i]; i++)
		free(__environ[i]);

	__environ = newEnviron;
	return (0);
}

/**
 * isPrefix - chekcks that "prefix" is equal to the part from the
 * beginning of "str" till an '=' sign is found
 *
 * @name: the name to be checked if it's the variable name of str
 * @str: the "var=value" string to check if name is equal to it "var" part
 * Return: 1 if str is a valid "var=value" string and name is equal to the part
 * from the beginning of str till the '=' character, 0 otherwise.
*/
int isVarName(const char *name, const char *str)
{
	int i, varLen; /* varLen is the length of the variable name in str */
	char strIsValid; /* a flag to be raised if str has an = character */

	/* Make sure str is valid & the var names has the same length */
	for (strIsValid = 0, varLen = 0; str[varLen]; varLen++)
		if (str[varLen] == '=')
		{
			strIsValid = 1;
			break;
		}
	if (!strIsValid)
		return (0);
	if (varLen != _strlen(name))
		return (0);

	/* Make sure they're identical */
	for (i = 0; i < varLen; i++)
		if (name[i] != str[i])
			return (0);

	/* passed the tests */
	return (1);
}

int main(int ac, char **av, char **envp)
{

    if (ac != 4)
    {
        printf("Usage: env varName varValue overwrite\n");
        exit(EXIT_FAILURE);
    }

    char var[] = "Ahmed=Programmer";
    free(__environ[2]);
    __environ[2] = var;

    // _setenv(envp, av[1], av[2], equal(av[3], "1"));
    for (int i = 0; __environ[i]; i++)
        printf("env[%d]: \"%s\"\n", i, __environ[i]);

    

    return(0);
}
