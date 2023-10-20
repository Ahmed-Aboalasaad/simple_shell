#include "main.h"

void freeCommand(Command *command)
{
	if (command)
	{
		if (command->str)
		{
			free(command->str);
			command->str = NULL;
		}
		if (command->argv)
		{
			int i;

			for (i = 0; command->argv[i]; i++)
			{
				free(command->argv[i]);
				command->argv[i] = NULL;
			}
			free(command->argv);
			command->argv = NULL;
		}
		free(command);
		command = NULL;
	}
}

void freeargv(Command *command)
{
	if (command)
	{
		if (command->argv)
		{
			int i;

			for (i = 0; command->argv[i]; i++)
			{
				free(command->argv[i]);
				command->argv[i] = NULL;
			}
			free(command->argv);
			command->argv = NULL;
		}
		free(command);
		command = NULL;
	}
}

void freeScript(Script *script)
{
	free(script->str);
	if (script->commands)
	{
		int i;
		for (i = 0; script->commands[i]; i++)
			freeargv(script->commands[i]);
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
		{
			/*print(1, script->commands[i]->str);*/
			/*if (script->commands[i]->str[0] != '\n' && script->commands[i]->argv[0] != NULL)*/
			/*if (script->commands[i]->str)*/
				/*print(1, script->commands[i]->str);*/
			if (script->commands[i]->argv[0] != NULL)
				executeCommand(script->commands[i], av[0], &commandID, script);
		}
		if (script)
			freeScript(script);
	}
	return (0);
}
/*freeCommand(script->commands[i]);*/
/*printf("\nHERE: %s\n", script->commands[0]->argv[0]);*/

/*
 *int main2(void)
 *{
 *	int i;
 *
 *	printf("==========================    Before    =================\n");
 *	for (i = 0; __environ[i]; i++)
 *		printf("%d) %s\n", i, __environ[i]);
 *
 *	_setenv("SHELL", "new shell :)", 1);
 *	printf("==========================    After     =================\n");
 *	for (i = 0; __environ[i]; i++)
 *		printf("%d) %s\n", i, __environ[i]);
 *	printf("=========================================================\n");
 *
 *	return (0);
 *}
 */
