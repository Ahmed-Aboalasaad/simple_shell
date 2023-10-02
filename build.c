#include "main.h"

void subScriptBuilder(int, char **, Script *, size_t *);

/**
 * buildCommand - finds out all the values of a command (reads its string,
 * makes argv[] for it, and sets flags for build in commands)
 *
 * @cmd: pointer to the command pointer
 * @line: the command line (if you have it). Set it to NULL if you don't
 * @commandCount: command counter
 * Return: 0 for success (There is a command to execute),
 * 1 for: the user inputted "" ... prompt the user to write again
*/
int buildCommand(Command **cmd, char *line, size_t *commandCount)
{
	size_t strSize;
	long charCount;
	Command *command;

	(*commandCount)++;
	*cmd = malloc(sizeof(**cmd));
	if (!*cmd)
	{
		perror("Allocation");
		exit(EXIT_FAILURE);
	}
	command = *cmd;

	/* Read the line (either from stdin or the given argument) */
	if (!line)
	{
		charCount = _getline(&command->str, &strSize, STDIN_FILENO);
		if (charCount == -1) /* Reading error */
		{
			free(command);
			exit(EXIT_FAILURE);
		}
		if (charCount == -2) /* Closing the shell */
		{
			free(command);
			print(STDOUT_FILENO, "\n");
			exit(EXIT_SUCCESS);
		}
		if (!(command->str[0])) /* Empty Command */
		{
			free(command->str);
			free(command);
			return (1);
		}
	}
	else
		command->str = line;

	command->argv = slice(command->str, " ");
	return (0);
}

/**
 * buildScript - fills the fields of a script instance of the Script structure.
 * It read the whole input (file / redirection) in one string, and slices it
 * into multiple commands in script->lines
 *
 * @s: pointer to the script instance
 * @commandCount: command counter
 * Return: 0 for success, 1 if an empty script was provided
 */
int buildScript(Script **s, size_t *commandCount)
{
	int charCount, n;
	size_t strSize;
	char **lines;
	Script *script;

	/* make a script object */
	*s = malloc(sizeof(**s));
	if (!*s)
		exit(EXIT_FAILURE);
	script = *s;

	/* Read the script (multiple lines) */
	charCount = _getline(&script->str, &strSize, STDIN_FILENO);
	if (charCount == -1) /* Reading Error */
	{
		free(script);
		exit(EXIT_FAILURE);
	}
	if (charCount == -2) /* Empty Script */
	{
		free(script);
		return (1);
	}

	/* slice the script into lines & build the commands using them */
	lines = slice(script->str, "\n");
	if (!lines) /* Allocation Error in slice() */
	{
		free(script->str);
		free(script);
		exit(EXIT_FAILURE);
	}
	for (n = 0; lines[n]; )
		n++;
	script->commands = malloc(sizeof(*script->commands) * (n + 1));
	subScriptBuilder(n, lines, script, commandCount);

	return (0);
}

/**
 * subScriptBuilder - extension for buildScript() as it exceeded 40 lines
 * and we need to satisfy betty
 *
 * @n: #commands in the script
 * @lines: an array holding the command lines
 * @script: pointer to the script instance
 * @commandCount: command counter
 * Return: nothing;
*/
void subScriptBuilder(int n, char **lines, Script *script
, size_t *commandCount)
{
	int i;

	if (!script->commands)
	{
		free(script->str);
		for (i = 0; lines[i]; i++)
			free(lines[i]);
		free(lines);
		free(script);
		exit(EXIT_FAILURE);
	}
	script->commands[n] = NULL;

	for (i = 0; i < n; i++)
		buildCommand(&(script->commands[i]), lines[i], commandCount);

	for (i = 0; lines[i]; i++)
		free(lines[i]);
	free(lines);
}

/**
 * _getline - reads a line from stream to string
 * and updates the size (n) to the new size
 *
 * @string: pointer to the string where it should write the read string
 * @n: pointer to the size of this allocated memory (string)
 * @stream: the stream it should read from
 * Return: #characters read if it succeeds,
 * -1 for errors,
 * -2 for EOF without any characters before
*/
long _getline(char **string, size_t *n, int stream)
{
	char *buffer, *tmp;
	int charCount, bufferedBefore;

	*string = malloc(1);
	*string[0] = '\0', buffer = malloc(BUFFER_SIZE);

	for (bufferedBefore = 0; 1; )
	{
		charCount = read(stream, buffer, BUFFER_SIZE - 1);
		if (charCount == 0 && !bufferedBefore)
		{
			free(*string);
			free(buffer);
			return (-2);
		}
		if (charCount == -1)
		{
			free(*string);
			free(buffer);
			perror("Read");
			return (-1);
		}
		bufferedBefore = 1;
		buffer[charCount] = '\0';
		if (charCount > 0 && buffer[charCount - 1] == '\n')
			buffer[charCount - 1] = '\0';
		tmp = *string;
		*string = concatStr(*string, buffer);
		free(tmp);
		if (charCount == 0)
		{
			free(buffer);
			return (charCount);
		}
		if (buffer[charCount - 1] == '\0')
			break;
	}
	free(buffer);
	*n = _strlen(*string);
	return (charCount);
}
