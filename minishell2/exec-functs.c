#include "minishell.h"


void or_cmd(int ret_val, s_cmd *prompt, int *i)
{
    (*i) += 2;
    if(*i >= prompt->nb_tabs)
    {
        ft_heredocpipex2(prompt, i, ret_val); //heredoc
        return;
    }
    if(!ret_val && *i + 1 < prompt->nb_tabs)
    {
        while(strcmp(prompt->cmd[*i].type, "and"))
            (*i)++;
    }
    else if(*i >= prompt->nb_tabs)
        return;
    if(!strcmp(prompt->cmd[*i].type, "char"))
        ft_firstcmd(ret_val, prompt, i, 0);
	else if(!strcmp(prompt->cmd[*i].type, "and"))
	{
		(*i)--;
        ft_and(prompt, i, ret_val);
	}
    else if(!strcmp(prompt->cmd[*i].tab, ">>"))
        ft_redirdoc(prompt->cmd[*i + 1].tab, prompt, i, 0);
    else if(!strcmp(prompt->cmd[*i].tab, "<<"))
        ft_heredoc(prompt, i);
    else if(!strcmp(prompt->cmd[*i].tab, ">"))
        ft_redirout(prompt, i);
    else if(!strcmp(prompt->cmd[*i].tab, "<"))
        ft_firstredirin(prompt, i);
}

void outfile_cmd(char *cmd, s_cmd *prompt, int *i, int infile)
{
    int outfile;
    (void)outfile;
    char **tab_outfile;

    while((*i) < prompt->nb_tabs && prompt->cmd[*i].tab[0] == '>')
    {
        (*i)++;
        tab_outfile = ft_split(prompt->cmd[*i].tab, ' ');
        outfile = open(tab_outfile[0], O_TRUNC | O_CREAT | O_WRONLY, 0644);
        (*i)++;
    }
    if(cmd)
        ft_execve(cmd, infile, prompt, i, outfile);
    if(*i < prompt->nb_tabs)
    {
        // if(!strcmp(prompt->cmd[*i].tab, "&&"))
        //     ft_and(prompt, i);
        if(!strcmp(prompt->cmd[*i].tab, "<"))
            ft_firstredirin(prompt, i);
        else if(!strcmp(prompt->cmd[*i].type, "pipe"))
        {
            (*i)++;
            ft_firstcmd(0, prompt, i, infile);
        }
    }
}

void ft_heredocpipex3(s_cmd *prompt, int *i, int ret_val)
{
	(void)i;
	(void)prompt;
	char *line;
	(void)line;
	(void)ret_val;
		
	line = readline(">");
	// if(ret_val)
	exec_lastcmddoc2(line ,prompt, 0, NULL, i);
	// waitpid(-1, NULL, 0);
}

void ft_heredocpipex2(s_cmd *prompt, int *i, int ret_val)
{
	(void)i;
	(void)prompt;
	char *line;
	(void)line;
	(void)ret_val;
		
	line = readline(">");
	// if(ret_val)
	exec_lastcmddoc(line ,prompt, 0, NULL, i);
	// waitpid(-1, NULL, 0);
}

void ft_heredocpipex(s_cmd *prompt, int *i)
{
	(void)i;
	(void)prompt;
	char *line;
	(void)line;
		
	line = readline(">");
	exec_lastcmddoc(line ,prompt, 0, NULL, i);
}

void ft_pipe(s_cmd *prompt, int *i)
{
	(*i)++;
	if(!strcmp(prompt->cmd[*i].type, "char"))
		ft_firstcmd(0, prompt, i, 0);
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
	if((*i) >= prompt->nb_tabs)
	{
		// printf("ft_heredocpipex2\n");
		ft_heredocpipex3(prompt, i, ret_value);
		return;
	}
	else if(!strcmp(prompt->cmd[*i + 1].type, "char"))
	{
		(*i)++;
		ft_firstcmd(ret_value, prompt, i, 0);
	}
}

void pipex_cmd(s_cmd *prompt, int *i, int *prevpipe)
{
	int fd[2];
	pid_t pid;
	char *path;
	char **args;
	(void)path;
	(void)args;

	// printf("cmd pipex: %s\n", prompt->cmd[(*i)].tab);
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
			ft_firstcmd(0, prompt, i, *prevpipe);
		wait(NULL);
	}
}