#include "main.h"

char *buildEnvStr(const char *name, const char *value);
char isVarName(const char *prefix, const char *str);

/**
 * _getenv - gets the value of an environment variable named var
 *
 * @name: the environment variable name
 * @index: int pointer to write the index of the env var (if any) to
 * Return: pointer on the variable value
 * or NULL if there is no such variable
*/
char *_getenv(const char *name, int *index)
{
	int i, j;

	for (i = 0; env[i]; i++)
		if (isVarName(name, env[i]))
		{
			*index = i;
			for (j = 0; env[i][j] != '=';)
				j++;
			return (&env[i][j + 1]);
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
int _setenv(const char *name, const char *value, int overwrite)
{
	char *oldValue, *newString, **newEnviron;
	int i, varCount, index;

	/* Input Validation */
	if (!name | !name[0] | contains(name, '='))
		return (1);

	newString = buildEnvStr(name, value), oldValue = _getenv(name, &index);
	if (!newString)
		return (1);
	if (oldValue) /* There is an environment variable with this name */
	{
		if (!overwrite)
		{
			free(newString);
			return (0);
		}
		/* update __environ[] */
		/* substitue the old "var=value" string with the new one */
		free(env[index]);
		env[index] = newString;
		return (0);
	}

	/* Adding a new Variable */
	/* Build & Fill a new env[] array*/
	for (varCount = 0; env[varCount]; )
		varCount++;
	newEnviron = malloc(sizeof(*newEnviron) * (varCount + 2));
	if (!newEnviron)
	{
		free(newString);
		return (1);
	}
	newEnviron[varCount + 1] = NULL;
	newEnviron[varCount] = newString;
	for (i = 0; i < varCount; i++)
		newEnviron[i] = copyStr(env[i]);
	/* Substitue the old environ with the new one */
	for (i = 0; env[i]; i++)
		free(env[i]);
	free(env);
	env = newEnviron;
	return (0);
}

/**
 * buildEnvStr - builds a "var=value" string ready to be added to __environ[]
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
	for (varNum = 0; env[varNum]; )
		varNum++;
	newEnviron = malloc(sizeof(*newEnviron) * varNum);
	if (!newEnviron)
		return (1);
	newEnviron[varNum - 1] = NULL;

	/* Fill it */
	for (i = j = 0; i < varNum; i++)
		if (j != index)
			newEnviron[i] = copyStr(env[j++]);

	/* Substitute the old __environ with the new one */
	for (i = 0; env[i]; i++)
		free(env[i]);

	env = newEnviron;
	return (0);
}

/**
 * isVarName - checks that "name" is equal to the part from the
 * beginning of "str" till an '=' sign is found
 *
 * @name: the name to be checked if it's the variable name of str
 * @envStr: the "var=value" string to check if name is equal to its "var" part
 * Return: 1 if str is a valid "var=value" string and name is equal to the part
 * from the beginning of str till the '=' character, 0 otherwise.
*/
char isVarName(const char *name, const char *envStr)
{
	int i, varLen; /* varLen is the length of the variable name in envStr */
	char strIsValid; /* a flag to be raised if str has an = character */

	/* Make sure str is valid & the var names has the same length */
	for (strIsValid = 0, varLen = 0; envStr[varLen]; varLen++)
		if (envStr[varLen] == '=')
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
		if (name[i] != envStr[i])
			return (0);

	/* passed the tests */
	return (1);
}
