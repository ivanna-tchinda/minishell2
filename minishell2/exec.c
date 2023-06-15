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

void ft_firstcmd(s_cmd *prompt, int *i, int infile)
{
	int prevpipe;

	prevpipe = dup(infile);
	if((*i) + 1 > prompt->nb_tabs)
		return;
	if((*i) + 1 == prompt->nb_tabs) //si c'est la derniere commande cmd
	{
		// printf("last cmd\n");
		exec_lastcmd(prompt, i, prevpipe, NULL);
	}
	else
	{
		// printf("cmd: %s\n", prompt->cmd[(*i)].tab);
		if(!strcmp(prompt->cmd[(*i) + 1].type, "pipe")) // cmd | ...
			pipex_cmd(prompt, i, &prevpipe);
		else if(!strcmp(prompt->cmd[(*i) + 1].type, "and")) // cmd && ...
		{
			// printf("here\n");
			exec_lastcmd(prompt, i, prevpipe, NULL);
			(*i) += 2;
			// printf("cmd: %s\n", prompt->cmd[(*i)].tab);
			ft_firstcmd(prompt, i, infile);
		}
		else if(!strncmp(prompt->cmd[(*i) + 1].tab, ">", ft_strlen(prompt->cmd[(*i) + 1].tab))) // cmd > outfile ...
		{
			// printf("outfile detected\n");
			(*i)++;
			outfile_cmd(prompt->cmd[(*i) - 1].tab, prompt, i, prevpipe);
		}
		else if(!strncmp(prompt->cmd[(*i) + 1].tab, "<", ft_strlen(prompt->cmd[(*i) + 1].tab)))
		{
			// printf("redirin\n");
			exec_lastcmd(prompt, i, prevpipe, NULL);
			(*i)++;
			ft_firstredirin(prompt, i);
		}
		else // cmd + false
			exec_lastcmd(prompt, i, infile, NULL);
	}
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
		(*i)++;
		prompt->cmd[*i].tab = tab_redir[1];
		ft_firstcmd(prompt, i, infile);
	}

	(*i) += 2;
	if((*i) >= prompt->nb_tabs) //si le redir est a la fin
		return;
	else if(!strncmp(prompt->cmd[*i].tab, "<", ft_strlen(prompt->cmd[*i].tab))) //si il y a un autre redirin apres
		ft_firstredirin(prompt, i);
	else if(!strcmp(prompt->cmd[*i].type, "pipe"))
		ft_pipe(prompt, i);
	else if(!strcmp(prompt->cmd[*i].type, "char"))
		ft_firstcmd(prompt, i, infile);
	else if(!strcmp(prompt->cmd[*i].type, "and"))
		ft_and(prompt, i);
	else if(!strncmp(prompt->cmd[(*i)].tab, ">", ft_strlen(prompt->cmd[(*i)].tab)))
	{
		ft_redirout(prompt, i);
	}
}

int ft_exec(s_cmd *prompt)
{
	int i;

	i = 0;
	if(!strcmp(prompt->cmd[i].type, "char")) //si on commance par une commande
		ft_firstcmd(prompt, &i, 0);
	else if(!strncmp(prompt->cmd[i].tab, "<", ft_strlen(prompt->cmd[i].tab))) //si on commence par un redirin
		ft_firstredirin(prompt, &i);
	//si on commence par un heredoc
	//si on commence par un redirout
	else if(!strncmp(prompt->cmd[i].tab, ">", ft_strlen(prompt->cmd[i].tab)))
		ft_redirout(prompt, &i);
	return(0);
}