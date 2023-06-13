#include "minishell.h"

void exec_lastcmd(s_cmd *prompt, int *i, int prevpipe)
{
	char *path;
	char **args;
	pid_t pid;

	path = ft_path(prompt->cmd[(*i)].tab, var_envir);
	args = ft_split(prompt->cmd[(*i)].tab, ' ');
	pid = fork();
	if(pid == 0)
	{
		dup2(prevpipe, STDIN_FILENO);
		close(prevpipe);
		if(execve(path, args, var_envir) == -1)
			write(2, "minishell: cmd not found\n", 25);
	}
	else
	{
		close(prevpipe);
		while (wait (NULL) != -1)
			;
	}
}

void pipex_cmd(s_cmd *prompt, int *i, int *prevpipe)
{
	int fd[2];
	pid_t pid;
	char *path;
	char **args;

	path = ft_path(prompt->cmd[(*i)].tab, var_envir);
	args = ft_split(prompt->cmd[(*i)].tab, ' ');
	pipe(fd);
	pid = fork();
	if(pid == -1)
		return;
	if(pid == 0)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		dup2(*prevpipe, STDIN_FILENO);
		close(*prevpipe);
		if(execve(path, args, var_envir) == -1)
			write(2, "minishell: cmd not found\n", 25);
	}
	else
	{
		close(fd[1]);
		close(*prevpipe);
		*prevpipe = fd[0];
		(*i) += 2;
		ft_firstcmd(prompt, i);
	}

}

void ft_firstcmd(s_cmd *prompt, int *i)
{
	int prevpipe;

	prevpipe = dup(0);
	if((*i) + 1 == prompt->nb_tabs) //si c'est la derniere commande
		exec_lastcmd(prompt, i, prevpipe);
	else if(!strcmp(prompt->cmd[(*i) + 1].type, "pipe"))
		pipex_cmd(prompt, i, &prevpipe);
	else if(!strcmp(prompt->cmd[(*i) + 1].type, "and"))
	{
		exec_lastcmd(prompt, i, prevpipe);
		(*i) += 2;
		ft_firstcmd(prompt, i);
	}
	
}

int ft_pipex(s_cmd *prompt)
{
	int i;

	i = 0;
	if(!strcmp(prompt->cmd[i].type, "char"))
		ft_firstcmd(prompt, &i);
	return(0);
}