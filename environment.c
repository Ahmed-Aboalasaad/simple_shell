#include "main.h"

char *buildVarStr(const char *name, const char *value);
char *_getenv(const char *var, int *index);
int isPrefix(const char *prefix, const char *str);

/**
 * _setenv - adds/modifies environmental variable
 *
 * @name: variable name
 * @value: variable value
 * @overwrite: modify the variable if it exists ?
 * Return: 0 on success, 1 for erros
*/
int _setenv(const char *name, const char *value, int overwrite)
{
	char *oldValue, *newString, **newEnviron;
	int i, varNum, index;

	varNum = 0, oldValue = _getenv(name, &index);
	if (oldValue) /* Ther is an environment variable with this name */
	{
		if (!overwrite)
			return (0);
		/* update __environ[] */
		/* Build the new "var=value" string" */
		newString = buildVarStr(name, value);
		if (!newString)
			return (1);
		/* substitue the old with the new "var=value" string */
		free(__environ[index]);
		__environ[index] = newString;
		return (0);
	}
	/* Adding a new Variable */
	/* Build & Fill a new __environ[] array*/
	newString = buildVarStr(name, value);
	if (!newString)
		return (1);
	for (varNum = 0; __environ[varNum]; )
		varNum++;
	newEnviron = malloc(sizeof(*newEnviron) * (varNum + 2));
	if (!newEnviron)
	{
		free(newString);
		return (1);
	}
	newEnviron[varNum + 1] = NULL;
	for (i = 0; i < varNum; i++)
		newEnviron[i] = __environ[i];
	newEnviron[i] = newString;
	/* Substitue the old environ with the new one */
	for (i = 0; __environ[i]; i++)
		free(__environ[i]);
	__environ = newEnviron;

	return (0);
}

/**
 * buildVarStr - builds a "var=value" string ready to be added to __environ[]
 *
 * @name: the variable name
 * @value: the variable value
 * Return: the new string, NULL otherwise
*/
char *buildVarStr(const char *name, const char *value)
{
	int i;
	char *result;

	result = malloc(sizeof(*result) * (_strlen(name) + _strlen(value) + 2));
	if (!result)
		return (NULL);
	result[_strlen(name) + _strlen(value) + 1] = '\0';

	for (i = 0; name[i]; i++)
		result[i] = name[i];
	result[i] = '=';
	for (i = 0; value[i]; i++)
		result[i] = value[i + _strlen(name) + 1];

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
 * _getenv - gets the value of an environment variable named var
 *
 * @var: the environment variable name
 * @index: int pointer to write the index of the env var (if any) to
 * Return: pointer on the variable value
 * or NULL if there is no such variable
*/
char *_getenv(const char *var, int *index)
{
	int i, j;

	for (i = 0; __environ[i]; i++)
		if (isPrefix(var, __environ[i]))
		{
			*index = i;
			for (j = 0; __environ[i][j] != '=';)
				j++;
			return (&__environ[i][j + 1]);
		}
	return (NULL);
}

/**
 * isPrefix - chekcks that "prefix" is equal to the part from the
 * beginning of "str" till an '=' sign is found
 *
 * @prefix: the prefix to be checked
 * @str: the string to check its prefix
 * Return: 1 if "prefix" is equal to the part from the beginning of "str" till
 * an '=' sign is found, 0 otherwise.
*/
int isPrefix(const char *prefix, const char *str)
{
	int i, prefixLen, strLen;

	/* Make sure they're the same length */
	for (i = prefixLen = 0; prefix[i]; i++)
		prefixLen++;
	for (i = strLen = 0; str[i] != '='; i++)
		strLen++;
	if (strLen != prefixLen)
		return (0);

	/* Make sure they have the same value */
	for (i = 0; str[i] != '='; i++)
		if (prefix[i] != str[i])
			return (0);

	/* passed the tests */
	return (1);
}
