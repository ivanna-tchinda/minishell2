#include "minishell.h"

void ft_firstcmd(int ret_val, s_cmd *prompt, int *i, int infile)
{
	int prevpipe;

	prevpipe = dup(infile);
	if((ret_val && !strcmp(prompt->cmd[*i - 1].type, "or")))
		ret_val = exec_lastcmd(prompt, i, prevpipe, NULL);
	else if(!ret_val)
	{
		if(*i + 1 < prompt->nb_tabs)
		{
			if(strcmp(prompt->cmd[*i + 1].type, "redir") && strcmp(prompt->cmd[*i + 1].type, "pipe"))
				ret_val = exec_lastcmd(prompt, i, prevpipe, NULL);
		}
		else if(*i - 1 > 0)
		{
			if(strcmp(prompt->cmd[*i - 1].type, "or"))
				ret_val = exec_lastcmd(prompt, i, prevpipe, NULL);

		}
		else
			ret_val = exec_lastcmd(prompt, i, prevpipe, NULL);
	}
	if((*i) + 1 >= prompt->nb_tabs) //si c'est la derniere commande cmd
		return;
	else
	{
		if(!strcmp(prompt->cmd[(*i) + 1].type, "or")) // cmd | ...
			or_cmd(ret_val, prompt, i);
		else if(!strcmp(prompt->cmd[(*i) + 1].type, "pipe")) // cmd | ...
			pipex_cmd(prompt, i, &prevpipe);
		else if(!strcmp(prompt->cmd[(*i) + 1].type, "and")) // cmd && ...
			ft_and(prompt, i, ret_val);
		else if(!strncmp(prompt->cmd[*i + 1].tab, ">>", ft_strlen(prompt->cmd[*i].tab)))
		{
			(*i)++;
			ft_redirdoc(prompt->cmd[*i - 1].tab, prompt, i, prevpipe);
		}
		else if(!strncmp(prompt->cmd[(*i) + 1].tab, ">", ft_strlen(prompt->cmd[(*i) + 1].tab))) // cmd > outfile ...
		{
			(*i)++;
			outfile_cmd(prompt->cmd[(*i) - 1].tab, prompt, i, prevpipe);
		}
		else if(!strncmp(prompt->cmd[(*i) + 1].tab, "<", ft_strlen(prompt->cmd[(*i) + 1].tab)))
		{
			exec_lastcmd(prompt, i, prevpipe, NULL);
			(*i)++;
			ft_firstredirin(prompt, i);
		}
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
		ft_firstcmd(0, prompt, i, infile);
	}

	(*i) += 2;
	if((*i) >= prompt->nb_tabs) //si le redir est a la fin
		return;
	else if(!strncmp(prompt->cmd[*i].tab, "<", ft_strlen(prompt->cmd[*i].tab))) //si il y a un autre redirin apres
		ft_firstredirin(prompt, i);
	else if(!strcmp(prompt->cmd[*i].type, "pipe"))
		ft_pipe(prompt, i);
	else if(!strcmp(prompt->cmd[*i].type, "char"))
		ft_firstcmd(0, prompt, i, infile);
	else if(!strcmp(prompt->cmd[*i].type, "and"))
		ft_and(prompt, i, 0);
	else if(!strncmp(prompt->cmd[(*i)].tab, ">", ft_strlen(prompt->cmd[(*i)].tab)))
		ft_redirout(prompt, i);
}


void ft_heredoc(s_cmd *prompt, int *i)
{
	char *delim;
	char *line;
	char **nospace;
	int filestdin;

	(*i)++;
	if(*i >= prompt->nb_tabs)
		nospace = NULL;
	else
		nospace = ft_split(prompt->cmd[*i].tab, ' ');
	filestdin = open("/dev/stdin", O_RDONLY);
	dup(filestdin);
	delim = NULL;
	if(nospace)
		delim = nospace[0];
	while(1)
	{
		line = readline(">");
		if(delim && !strncmp(line, delim, ft_strlen(delim)))
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
			ft_and(prompt, i, 0);
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
		ft_execve(cmd, infile, prompt, i, outfile);
	return;


}


void ft_redirout(s_cmd *prompt, int *i)
{
    int outfile;
    char **tab_otf;
    (void)outfile;

    (*i)++;
    tab_otf = ft_split(prompt->cmd[*i].tab, ' ');
    outfile = open(tab_otf[0], O_TRUNC | O_CREAT | O_WRONLY, 0644);
    if(prompt->cmd[*i + 1].type)
        (*i)++;
    if(*i >= prompt->nb_tabs)
        return;
    else if(prompt->cmd[*i].tab[0] == '>')
        ft_redirout(prompt, i);
    else if(!strcmp(prompt->cmd[*i].tab, "<"))
        ft_firstredirin(prompt, i);
    else if(!strcmp(prompt->cmd[*i].type, "char"))
        ft_firstcmd(0, prompt, i, 0);
    else if(!strcmp(prompt->cmd[*i].type, "and"))
        ft_and(prompt, i, 0);
    else if(!strcmp(prompt->cmd[*i].type, "pipe"))
        ft_pipe(prompt, i);
}

int ft_exec(s_cmd *prompt, s_token *token)
{
	int i;
	s_cmd par_prompt;

	i = 0;
	if(!prompt->nb_cmd)
		return 1;
	if(!strcmp(prompt->cmd[i].type, "parentheses")) //si on commance par des parentheses
		ft_parentheses(prompt, &i, token, &par_prompt);
	else if(!strcmp(prompt->cmd[i].type, "char")) //si on commance par une commande
		ft_firstcmd(0, prompt, &i, 0);
	else if(!strncmp(prompt->cmd[i].tab, "<<", ft_strlen(prompt->cmd[i].tab))) //si on commence par un heredoc <<
		ft_heredoc(prompt, &i);
	else if(!strncmp(prompt->cmd[i].tab, ">>", ft_strlen(prompt->cmd[i].tab))) //si on commence par un heredoc >>
		ft_redirdoc(NULL, prompt, &i, 0);
	else if(!strncmp(prompt->cmd[i].tab, "<", ft_strlen(prompt->cmd[i].tab))) //si on commence par un redirin
		ft_firstredirin(prompt, &i);
	else if(!strncmp(prompt->cmd[i].tab, ">", ft_strlen(prompt->cmd[i].tab))) //si on commence par un redirin
		ft_redirout(prompt, &i);
	return(0);
}