#include "minishell.h"


char *after_infile(char *prompt)
{
	int i;

	i = -1;
	while(prompt[++i] == 32);
	while(prompt[++i] && prompt[i] != 32);
	return(ft_substr(prompt, i, ft_strlen(prompt) - i));
}

void ft_pipe(s_cmd *prompt, int *i)
{
	(*i)++;
	if(!strcmp(prompt->cmd[*i].type, "char"))
		ft_firstcmd(prompt, i, 0);
	else if(!strcmp(prompt->cmd[*i].tab, ">"))
	{
		// printf("ici %d, %s\n", *i, prompt->cmd[*i].tab);
		ft_redirout(prompt, i);
	}
	else if(!strcmp(prompt->cmd[*i].tab, "<"))
		ft_firstredirin(prompt, i);
	else 
	{
		(*i)++;
		ft_heredoc(prompt, i);
	}
}

void ft_and(s_cmd *prompt, int *i, int ret_value)
{
	(*i)++;
	if(ret_value)
		return;
	printf("%d and cmd: %s\n", *i, prompt->cmd[*i].tab);
	if((*i) == prompt->nb_tabs)
		ft_heredocpipex(prompt, i);
	else if(!strcmp(prompt->cmd[*i].type, "char"))
		ft_firstcmd(prompt, i, 0);
	else if(!strcmp(prompt->cmd[*i].type, "pipe"))
		ft_pipe(prompt, i);
}

void ft_execve(char *cmd, int prevpipe, s_cmd *prompt, int *i)
{
	char *path;
	char **args;
	pid_t pid;
	(void)i;

	cmd = without_spaces(cmd);
	printf("cmd: %s\n", cmd);
	path = ft_path(cmd, var_envir);
	args = ft_split(cmd, ' ');
	pid = fork();
	if(pid == 0)
	{
		
		dup2(prevpipe, STDIN_FILENO);
		close(prevpipe);
		if(is_builtin(cmd))
			exec_bltn(cmd, prompt);
		else if(execve(path, args, var_envir) == -1)
		{
			free(path);
			write(2, "minishell: cmd not found\n", 25);
			exit(0);
		}
	}
	else
	{
		close(prevpipe);
		wait(NULL);
		// exit(0);
	}
	return;
}

int exec_lastcmddoc(char *cmd, s_cmd *prompt, int prevpipe, char *outfile, int *i)
{
	(void)i;
	char *path;
	char **args;
	pid_t pid;

	path = ft_path(cmd, var_envir);
	args = ft_split(cmd, ' ');
	pid = fork();
	if(pid == 0)
	{
		if(outfile)
		{
			int out;
			out = open(outfile, O_TRUNC | O_CREAT | O_WRONLY, 0644);
			// printf("out: %d\n", out);
			dup2(out, STDOUT_FILENO);
			close(out);
		}
		dup2(prevpipe, STDIN_FILENO);
		close(prevpipe);
		if(is_builtin(cmd))
			exec_bltn(cmd, prompt);
		else if(execve(path, args, var_envir) == -1)
		{
			write(2, "minishell: cmd not found\n", 25);
			// exit(1);
			// return(1);
		}
	}
	else
	{
		close(prevpipe);
		wait(NULL);
		// printf("i: %d\n", *i);
		exit(0);
		// ft_firstcmd(prompt, i, 0);

	}
	return(0);
}

int exec_lastcmd(s_cmd *prompt, int *i, int prevpipe, char *outfile)
{
	char *path;
	char **args;
	int ret = 0;
	pid_t pid;

	path = ft_path(prompt->cmd[(*i)].tab, var_envir);
	args = ft_split(prompt->cmd[(*i)].tab, ' ');
	pid = fork();
	if(pid == 0)
	{
		if(outfile)
		{
			int out;
			out = open(outfile, O_TRUNC | O_CREAT | O_WRONLY, 0644);
			// printf("out: %d\n", out);
			dup2(out, STDOUT_FILENO);
			close(out);
		}
		dup2(prevpipe, STDIN_FILENO);
		close(prevpipe);
		if(is_builtin(prompt->cmd[(*i)].tab))
			exec_bltn(prompt->cmd[(*i)].tab, prompt);
		else if(execve(path, args, var_envir) == -1)
		{
			write(2, "minishell: cmd not found\n", 25);
			ret = 1;
			// exit(1);
			return(1);
		}
	}
	else
	{
		close(prevpipe);
		wait(NULL);
	}
	return(ret);
}

void pipex_cmd(s_cmd *prompt, int *i, int *prevpipe)
{
	int fd[2];
	pid_t pid;
	char *path;
	char **args;
	(void)path;
	(void)args;

	printf("cmd pipex: %s\n", prompt->cmd[(*i)].tab);
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
		if(is_builtin(prompt->cmd[(*i)].tab))
			exec_bltn(prompt->cmd[(*i)].tab, prompt);
		else if(execve(path, args, var_envir) == -1)
		{
			write(2, "minishell: cmd not found\n", 25);
			exit(1);
		}
	}
	else
	{
		close(fd[1]);
		close(*prevpipe);
		*prevpipe = fd[0];
		(*i) += 2;
		// if(*i >= prompt->nb_tabs)
		// 	ft_heredocpipex(prompt, i);
		// else
			ft_firstcmd(prompt, i, *prevpipe);
	}
}