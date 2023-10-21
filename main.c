#include "main.h"

int main(int ac, char **av);
int previousExitValue;
char *shellName;
char **env;
size_t commandID;

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
	Script *script;
	Command *command;
	int i;

	/* Initial Values */
	(void)ac;
	commandID = 0;
	script = NULL;
	command = NULL;
	signal(SIGINT, interruption);
	/* a local copy of the environment variables array */
	for (i = 0; __environ[i]; )
		i++;
	env = malloc(sizeof(*env) * (i + 1));
	if (!env)
		exit(5);
	env[i] = NULL;
	for (i = 0; __environ[i]; i++)
		env[i] = copyStr(__environ[i]);
	/* Interactive mode */
	if (isatty(STDIN_FILENO))
	{
		while (1)
		{
			print(STDOUT_FILENO, "#cisfun$ ");
			command = buildCommand(NULL);
			if (command->argv[0])
				executeCommand(command, NULL);
			if (command)
				freeCommand(command);
		}
	}
	else /* non-interactive mode */
	{
		if (buildScript(&script))
			return (0); /* Empty Script */
		for (i = 0; script->commands[i]; i++)
			if (script->commands[i]->argv[0] != NULL)
				executeCommand(script->commands[i], script);
		if (script)
			freeScript(script);
	}
	for (i = 0; env[i]; i++)
		free(env[i]);
	free(env);
	return (previousExitValue);
}
