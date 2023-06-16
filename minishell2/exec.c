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
		else if(!strncmp(prompt->cmd[*i + 1].tab, ">>", ft_strlen(prompt->cmd[*i].tab)))
		{
			printf("redirdoc\n");
			(*i)++;
			ft_redirdoc(prompt->cmd[*i - 1].tab, prompt, i, prevpipe);
		}
		// else // cmd + false
		// 	exec_lastcmd(prompt, i, infile, NULL);
	}
}


void ft_firstredirin(s_cmd *prompt, int *i)
{
	int infile;
	char **tab_redir;

	printf("redir\n");
	printf("redir tab[%d]: %s\n", *i, prompt->cmd[(*i)].tab);
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

void ft_heredoc(s_cmd *prompt, int *i)
{
	char *delim;
	char *line;
	char **nospace;
	int filestdin;

	(*i)++;
	filestdin = open("/dev/stdin", O_RDONLY);
	dup(filestdin);
	nospace = ft_split(prompt->cmd[*i].tab, ' ');
	delim = nospace[0];
	while(1)
	{
		line = readline(">");
		if(!strncmp(line, delim, ft_strlen(delim)))
			break;
	}
	if(prompt->cmd[*i + 1].type)
	{
		(*i)++;
		if(!strncmp(prompt->cmd[(*i)].tab, ">", ft_strlen(prompt->cmd[(*i)].tab)))
			ft_redirout(prompt, i);
		else if(!strncmp(prompt->cmd[*i].tab, "<", ft_strlen(prompt->cmd[*i].tab)))
			ft_firstredirin(prompt, i);
		else if(!strcmp(prompt->cmd[*i].type, "and"))
			ft_and(prompt, i);
		else if(!strcmp(prompt->cmd[*i].type, "pipe"))
			ft_pipe(prompt, i);
	}
}

void ft_redirdoc(char *cmd, s_cmd *prompt, int *i, int infile)
{
	char **doc;
	int outfile;
	(void)infile;
	(void)outfile;
	(void)cmd;


	while((*i) < prompt->nb_tabs && !strcmp(prompt->cmd[*i].tab, ">>"))
	{
		(*i)++;
		doc = ft_split(prompt->cmd[*i].tab, ' ');
		outfile = open(doc[0], O_CREAT | O_WRONLY | O_APPEND, 0664);
		(*i)++;
	}
	if(cmd)
		ft_execve(cmd, infile, prompt, outfile);
	return;


}

int ft_exec(s_cmd *prompt)
{
	int i;

	i = 0;
	if(!strcmp(prompt->cmd[i].type, "char")) //si on commance par une commande
		ft_firstcmd(prompt, &i, 0);
	//si on commence par un heredoc <<
	else if(!strncmp(prompt->cmd[i].tab, "<<", ft_strlen(prompt->cmd[i].tab)))
		ft_heredoc(prompt, &i);
	else if(!strncmp(prompt->cmd[i].tab, ">>", ft_strlen(prompt->cmd[i].tab)))
		ft_redirdoc(NULL, prompt, &i, 0);
	else if(!strncmp(prompt->cmd[i].tab, "<", ft_strlen(prompt->cmd[i].tab))) //si on commence par un redirin
		ft_firstredirin(prompt, &i);
	else if(!strncmp(prompt->cmd[i].tab, ">", ft_strlen(prompt->cmd[i].tab)))
		ft_redirout(prompt, &i);
	//si on commence par un heredoc >>
	return(0);
}