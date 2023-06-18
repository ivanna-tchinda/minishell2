#include "minishell.h"

void or_cmd(int ret_val, s_cmd *prompt, int *i)
{
    (*i) += 2;
    if(!ret_val && *i < prompt->nb_tabs)
    {
        while(strcmp(prompt->cmd[*i].type, "and"))
            (*i)++;
    }
    printf("i %d\n", *i);
    if(*i >= prompt->nb_tabs)
    {
        ft_heredocpipex2(prompt, i, ret_val); //heredoc
        return;
    }
    else if(*i >= prompt->nb_tabs)
        return;
    if(!strcmp(prompt->cmd[*i].type, "char"))
        ft_firstcmd(prompt, i, 0);
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
        ft_execve(cmd, infile, prompt, i);
    if(*i < prompt->nb_tabs)
    {
        // if(!strcmp(prompt->cmd[*i].tab, "&&"))
        //     ft_and(prompt, i);
        if(!strcmp(prompt->cmd[*i].tab, "<"))
            ft_firstredirin(prompt, i);
        else if(!strcmp(prompt->cmd[*i].type, "pipe"))
        {
            (*i)++;
            ft_firstcmd(prompt, i, infile);
        }
    }
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
        ft_firstcmd(prompt, i, 0);
    // else if(!strcmp(prompt->cmd[*i].type, "and"))
    //     ft_and(prompt, i);
    else if(!strcmp(prompt->cmd[*i].type, "pipe"))
        ft_pipe(prompt, i);
}