#include "main.h"

void buildScript2(char **lines,
					  Script * script, size_t *commandID);

/**
 * buildScript - fills the fields of a script instance of the Script structure.
 * It read the whole input (file / redirection) in one string, and slices it
 * into multiple commands in script->lines
 *
 * @s: pointer to the script instance
 * @commandID: command counter
 * Return: 0 for success, 1 if an empty script was provided
 */
int buildScript(Script **s, size_t *commandID)
{
	int charCount;
	char **lines;
	Script *script;

	/* make a script object */
	*s = malloc(sizeof(**s));
	if (!*s)
		exit(EXIT_FAILURE);
	script = *s;

	/* Read the script (many lines) */ /* needs fixing (reading line by line) */
	charCount = readLine(&script->str, STDIN_FILENO);
	if (charCount == -1) /* Reading Error */
	{
		free(script);
		exit(EXIT_FAILURE);
	}
	if (charCount == 0) /* Empty Script */
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
	buildScript2(lines, script, commandID);

	return (0);
}

/**
 * subScriptBuilder - extension for buildScript() as it exceeded 40 lines
 * and we need to satisfy betty :L)
 *
 * @lines: an array holding the command lines
 * @script: pointer to the script instance
 * @commandID: command counter
 * Return: nothing;
*/
void buildScript2(char **lines,
					  Script *script, size_t *commandID)
{
	int i, commandCount;

	for (commandCount = 0; lines[commandCount]; )
		commandCount++;
	script->commands = malloc(sizeof(*script->commands) * (commandCount + 1));
	if (!script->commands)
	{
		free(script->str);
		for (i = 0; lines[i]; i++)
			free(lines[i]);
		free(lines);
		free(script);
		exit(EXIT_FAILURE);
	}
	script->commands[commandCount] = NULL;

	for (i = 0; i < commandCount; i++)
		script->commands[i] =  buildCommand(lines[i], commandID);

	for (i = 0; lines[i]; i++)
		free(lines[i]);
	free(lines);
}
