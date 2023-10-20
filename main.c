#include "main.h"

int main(int ac, char **av);
int previousExitValue;
char **env;

void freeCommand(Command *command)
{
	int i;

	if (!command)
		return;
	
	if (command->str)
	{
		free(command->str);
		command->str = NULL;
	}
	if (command->argv)
	{
		for (i = 0; command->argv[i]; i++)
			free(command->argv[i]);
		free(command->argv);
	}
	free(command);
	command = NULL;
}

void freeScript(Script *script)
{
	int i;

	free(script->str);
	if (script->commands)
	{
		for (i = 0; script->commands[i]; i++)
			freeCommand(script->commands[i]);
		free(script->commands);
	}
	free(script);
	script = NULL;
}

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
			command = buildCommand(NULL, &commandID);
			if (command->argv[0])
				executeCommand(command, av[0], &commandID, NULL);
			if (command)
				freeCommand(command);
		}
	}
	else /* non-interactive mode */
	{
		if (buildScript(&script, &commandID)) /* Empty Script */
			return (0); 
		for (i = 0; script->commands[i]; i++)
			if (script->commands[i]->argv[0] != NULL)
				executeCommand(script->commands[i], av[0], &commandID, script);
		if (script)
			freeScript(script);
	}
	for (i = 0; env[i]; i++)
		free(env[i]);
	free(env);
	return (0);
}
