#include "main.h"

void subScriptBuilder(int, char **, Script *, size_t *);

/**
 * buildCommand - finds out all the values of a command
 * (Reads its string, and Makes argv[] for it)
 *
 * @cmd: pointer to a command pointer
 * @str: the command line (if you have it). Set it to NULL if you don't
 * @commandID:pointer to the commandID variable in main()
 * it represents #commands done on this session
 *
 * Return:
 * 0 for success (There is a command to execute & it was built normally),
 * 1 if the it read empty input("")
 * In htis case, you you should execute nothing & prompt again
*/
int buildCommand(Command **cmd, char *str, size_t *commandID)
{
	size_t commandSize;
	long charCount;
	Command *command;

	/* Allocation */
	(*commandID)++;
	*cmd = malloc(sizeof(**cmd));
	if (!*cmd)
	{
		perror("Allocation");
		exit(EXIT_FAILURE);
	}
	command = *cmd;
	command->str = command->argv = NULL;

	/* Fill the fields */
	if (str)
	{
		command->str = str, command->argv = slice(command->str, " ");
		return (0);
	}
	charCount = _getline(&command->str, &commandSize, STDIN_FILENO);
	if (charCount == -1) /* Reading error */
	{
		free(command);
		exit(EXIT_FAILURE);
	}
	if (charCount == -2) /* EOF without any chars before.. Closing the shell */
	{
		free(command);
		print(STDOUT_FILENO, "\n");
		exit(EXIT_SUCCESS);
	}
	if (!(command->str[0])) /* Empty Command ("") */
	{
		free(command->str);
		free(command);
		return (1);
	}
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

long _getline(char **string, size_t *size, int stream)
{
	char *buffer, *tmp;
	int charCount, bufferedBefore;

	if (*string)
		free(*string);
	*string = malloc(1), *string[0] = '\0', buffer = malloc(BUFFER_SIZE);

	for (bufferedBefore = 0; 1; bufferedBefore = 1)
	{
		charCount = read(stream, buffer, BUFFER_SIZE - 1);
		if (charCount < 1)
		{
			free(*string);
            *string = NULL;
			free(buffer);
			if (charCount == -1)
				return (-1); /* Reading Error or Signal Interruption */
			if (!bufferedBefore) /* reaching here means charCount == 0 */
				return (-2); /* EOF without any characters before */
		}
		buffer[charCount] = '\0';
		tmp = *string;
		*string = concatStr(*string, buffer);
		free(tmp);
		if (charCount < BUFFER_SIZE - 1)
			break;
	}
	free(buffer);
	*size = _strlen(*string);
	return (charCount);
}
