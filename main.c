#include "main.h"

/**
 * main - simple shell
 *
 * @ac: argument count
 * @av: argument vector
 * Return: 0 for success, non-zero value for errors
*/
int main(int ac, char **av)
{
	size_t commandCount;
	Script *script;
	Command *command;
	int i;

	/* Initial Values */
	(void)ac;
	commandCount = 0;
	script = NULL;

	/* Interactive mode */
	if (isatty(STDIN_FILENO))
	{
		while (1)
		{
			print(STDOUT_FILENO, "#cisfun$ ");
			if (buildCommand(&command, NULL, &commandCount))
				continue;
			executeCommand(command, av[0], &commandCount, 1);
		}
	}
	/* non-interactive mode */
	else
	{
		if (buildScript(&script, &commandCount)) /* Empty Script */
			return (0);
		for (i = 0; script->commands[i]; i++)
			executeCommand(script->commands[i], av[0], &commandCount, 0);
	}
	return (0);
}
