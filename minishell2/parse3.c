#include "minishell.h"

char *substr_cmd(s_token *token, int start, int end)
{
    char *cmd;
    int i;

    cmd = malloc(sizeof(char) * end - start);
    i = 0;
    if(start == end)
        return(&token[start].token);
    while(start < end)
    {
        cmd[i] = token[start].token;
        i++;
        start++;
    }
    cmd[i] = '\0';
    return(cmd);
}

void add_totab(s_info *cmd, s_token *token, int *i)
{
    int j;

    while(token[(*i)].token == 32)
        (*i)++;
    j = (*i) + 1;
    cmd->type = token[(*i)].type;
    if(token[j].type == token[j-1].type)
    {
        while((token[j].type == token[j-1].type && token[j++].type));
        cmd->tab = substr_cmd(token, *i, j);
        *i = j;
    }
    else
    {
        j = (*i);
        if(token[*i].token == 34)
        {
            while(token[++j].token != 34 && token[*i].token);
            j++;
        }
        else if(token[*i].token == 39)
        {
            while(token[++j].token != 39);
            j++;
        }
        else if(token[j].type != token[j+1].type)
            cmd->tab = substr_cmd(token, (*i)++, j);
        j++;
    }
} //+25 mais ok

int check_and(s_info *cmd, int len_cmd)
{
    int i;

    i = -1;
    while(++i < len_cmd)
    {
        if(cmd[i].type && strcmp(cmd[i].type, "and") == 0)
        {
            if(i == 0)
                return 1;
            if(strcmp(cmd[i - 1].type, "char") != 0)
                    return(1);
        } 
    }
    return 0;
}

int parse_command(s_cmd *prompt, int len_cmd)
{
    int i;

    i = 0;
    if(prompt->nb_cmd == 0)
        return (1);
    while(i < len_cmd)
    {
        //cas1: pipe sans arg avant ou apres
        if(check_pipe(prompt->cmd, len_cmd))
            return(write(1, "zsh: parse error near `|'\n", 26));
        //cas2: redirection sans arg avant ou apres
        else if(check_and(prompt->cmd, len_cmd))
            return(write(1, "syntax error near unexpected token `&'\n", 39));
        else if(check_redif(prompt->cmd, len_cmd))
            return(write(1, "zsh: parse error\n", 17));
        i++;
    }
    return(0);
}

int only_cmd(s_cmd *prompt)
{
    int i;
    int count;

    i = 0;
    count = 0;
    if(!prompt->nb_tabs)
        return(0);
    while(i < prompt->nb_tabs)
    {
        if(!prompt->cmd[i].type)
            break;
        if(strcmp(prompt->cmd[i].type, "char") != 0)
            count++;
        i++;
    }
    if(strcmp(prompt->cmd[i - 1].type, "char") == 0)
        count++;
    return(count);
}