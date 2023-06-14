#include "minishell.h"

char *without_spaces(char *cmd)
{
	char **no_space;
	char *to_ret;
	int i;

	i = 0;
	no_space = ft_split(cmd, ' ');
	while(no_space[i])
	{
		i++;
	}
	if(i > 1)
	{
		i = 0;
		to_ret = NULL;
		while(no_space[i])
		{
			to_ret = ft_strjoin(to_ret, no_space[i]);
			to_ret = ft_strjoin(to_ret, " ");
			i++;
		}
	}
	else
		return(no_space[0]);
	return(to_ret);
}

void ft_execve(char *cmd, int prevpipe, s_cmd *prompt, char *outfile)
{
	char *path;
	char **args;
	pid_t pid;

	cmd = without_spaces(cmd);
	path = ft_path(cmd, var_envir);
	args = ft_split(cmd, ' ');
	pid = fork();
	if(pid == 0)
	{
		if(outfile)
		{
			int out;
			out = open(outfile, O_TRUNC | O_CREAT | O_WRONLY, 0644);
			dup2(out, STDOUT_FILENO);
			close(out);
		}
		dup2(prevpipe, STDIN_FILENO);
		close(prevpipe);
		if(is_builtin(cmd))
			exec_bltn(cmd, prompt);
		else if(execve(path, args, var_envir) == -1)
		{
			free(path);
			write(2, "minishell: cmd not found\n", 25);
			exit(1);
		}
	}
	else
	{
		close(prevpipe);
		wait(NULL);
	}
	return;
}

void exec_lastcmd(s_cmd *prompt, int *i, int prevpipe, char *outfile)
{
	char *path;
	char **args;
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
			exit(1);
		}
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
		if(is_builtin(prompt->cmd[(*i)].tab))
			exec_bltn(prompt->cmd[(*i)].tab, prompt);
		if(execve(path, args, var_envir) == -1)
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
		ft_firstcmd(prompt, i, *prevpipe);
	}

}

void ft_firstcmd(s_cmd *prompt, int *i, int infile)
{
	int prevpipe;

	prevpipe = dup(infile);
	if((*i) + 1 > prompt->nb_tabs)
		return;
	if((*i) + 1 == prompt->nb_tabs) //si c'est la derniere commande
		exec_lastcmd(prompt, i, prevpipe, NULL);
	else if(!strcmp(prompt->cmd[(*i) + 1].type, "pipe"))
		pipex_cmd(prompt, i, &prevpipe);
	else if(!strcmp(prompt->cmd[(*i) + 1].type, "and"))
	{
		exec_lastcmd(prompt, i, prevpipe, NULL);
		(*i) += 2;
		ft_firstcmd(prompt, i, infile);
	}
	else if(!strncmp(prompt->cmd[(*i) + 1].tab, ">", ft_strlen(prompt->cmd[(*i) + 1].tab)))
	{
		exec_lastcmd(prompt, i, prevpipe, prompt->cmd[(*i) + 2].tab);
		(*i) += 3;
		ft_firstcmd(prompt, i, infile);
	}
	
}

char *after_infile(char *prompt)
{
	int i;

	i = -1;
	while(prompt[++i] == 32);
	while(prompt[++i] && prompt[i] != 32);
	return(ft_substr(prompt, i, ft_strlen(prompt) - i));
}

void ft_and(s_cmd *prompt, int *i)
{
	(*i)++;
	if(!prompt->cmd[*i].tab)
		printf("heredoc spe\n");
	else if(!strcmp(prompt->cmd[*i].type, "char"))
		ft_firstcmd(prompt, i, 0);
}

void ft_firstredirin(s_cmd *prompt, int *i)
{
	int infile;
	char **tab_redir;

	if((*i) == prompt->nb_tabs) //si le redir est a la fin
		return;
	tab_redir = ft_split(prompt->cmd[(*i) + 1].tab, ' ');
	infile = open(tab_redir[0], O_RDONLY, 777);
	if(tab_redir[1])
	{
		if(!strncmp(prompt->cmd[(*i) + 2].tab, ">", ft_strlen(prompt->cmd[(*i) + 2].tab)))
		{
			ft_execve(after_infile(prompt->cmd[(*i) + 1].tab), infile, prompt, prompt->cmd[(*i) + 3].tab);
			(*i) += 4;
		}
		else
			ft_execve(after_infile(prompt->cmd[(*i) + 1].tab), infile, prompt, NULL);
	}
	(*i) += 2;
	if((*i) >= prompt->nb_tabs) //si le redir est a la fin
		return;
	else if(!strncmp(prompt->cmd[*i].tab, "<", ft_strlen(prompt->cmd[*i].tab))) //si il y a un autre redirin apres
		ft_firstredirin(prompt, i);
	else if(!strcmp(prompt->cmd[*i].type, "char"))
		ft_firstcmd(prompt, i, infile);
	else if(!strcmp(prompt->cmd[*i].type, "and"))
		ft_and(prompt, i);
}

int ft_pipex(s_cmd *prompt)
{
	int i;

	i = 0;
	if(!strcmp(prompt->cmd[i].type, "char")) //si on commance par une commande
		ft_firstcmd(prompt, &i, 0);
	else if(!strncmp(prompt->cmd[i].tab, "<", ft_strlen(prompt->cmd[i].tab))) //si on commence par un redirin
		ft_firstredirin(prompt, &i);
	//si on commence par un heredoc
	//si on commence par un redirout
	return(0);
}