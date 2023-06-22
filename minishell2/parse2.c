#include "minishell.h"

int parentheses(char *cmd)
{
    int i;
    int count;
    int par;

    i = 0;
    count = 0;
    par = 0;
    while(i < (int)ft_strlen(cmd))
    {
        if(cmd[i] && (cmd[i] == 40 || cmd[i] == 41))
            count++;
        if(cmd[i] && cmd[i] == 40 && cmd[i + 1] == 41)
            return (1);
        if(cmd[i] && cmd[i] == 40)
            par++;
        else if(cmd[i] && cmd[i] == 41)
            par--;
        if(par < 0)
            return(1);
        i++;
    }
    if(count % 2 != 0)
        return (1);
    return (0);
}

void quote_type(s_token *token, char *line, int *i)
{
    token[*i].token = line[*i];
    if(line[*i] == 34)
    {
        token[*i].type = "noexp$";
        while(line[++(*i)])
        {
            token[*i].type = "noexp$";
            token[*i].token = line[*i];
            if(line[*i] == 34)
                break;
        }
    }
    else if(line[*i] == 39)
    {
        token[*i].type = "noexp";
        while(line[++(*i)])
        {
            token[*i].type = "noexp";
            token[*i].token = line[*i];
            if(line[*i] == 39)
                break;
        }
    }
}


int ft_nbtokens(s_token *token)
{
    int i;
    int count;

    i = 0;
    count = 0;
    while(token[i].token)
    {
        if(token[i].type != token[i - 1].type && token[i].token)
        {
            // printf("%d token %c\n", i, token[i].token);
            count++;
        }
        i++;
    }
    return(count + 1);
}

int check_pipe(s_info *cmd, int len_cmd)
{
    int i;
    
    i = 0;
    while(i + 1 < len_cmd && cmd[i].type)
    {
        if(cmd[i].type && (strcmp(cmd[i].type, "pipe") == 0 || strcmp(cmd[i].type, "or") == 0))
        {
            if(i == 0)
                return 1;
            if(!cmd[i + 1].tab || !cmd[i - 1].tab)
                return(1);
            if(strcmp(cmd[i - 1].type, "char") && strcmp(cmd[i - 1].type, "parentheses"))
                return(1);
        }
        i++;
    }
    return(0);
}

int check_redif(s_info *cmd, int len_cmd)
{
    int i;

    i = 0;
    while(i < len_cmd && cmd[i].type)
    {
        if((cmd[i].type && !strcmp(cmd[i].type, "redir")))
        {
            if(i == (len_cmd - 1))
                return 1;
            if(!cmd[i + 1].type || !cmd[i - 1].type)
                return(1);
        }
        i++;
    }
    return(0);
}
