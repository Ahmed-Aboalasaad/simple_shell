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
	/* #commands done in this session */
	/* (incremented by 1 every time in input is read) */
	size_t commandID;
	Script *script;
	Command *command;
	int i;

	/* Initial Values */
	(void)ac;
	commandID = 0;
	script = NULL;
	command = NULL;
	signal(SIGINT, interruption);

	/* Interactive mode */
	if (isatty(STDIN_FILENO))
	{
		while (1)
		{
			print(STDOUT_FILENO, "#cisfun$ ");
			if (buildCommand(&command, NULL, &commandID))
				continue; /* empty command: ("\n") */
			executeCommand(command, av[0], &commandID, 1);
		}
	}
	else /* non-interactive mode */
	{
		if (buildScript(&script, &commandID)) /* Empty Script */
			return (0);
		for (i = 0; script->commands[i]; i++)
			executeCommand(script->commands[i], av[0], &commandID, 0);
	}
	return (0);
}
