#include "minishell.h"

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
            count++;
        i++;
    }
    // printf("nb of types: %d\n", count);
    return(count);
}

int check_pipe(s_info *cmd, int len_cmd)
{
    int i;
    
    i = 0;
    // printf("nb cmd: %d\n", len_cmd);
    printf("check pipe\n");
    while(i < len_cmd)
    {
        // printf("type: %s\n", cmd[i].type);
        if(cmd[i].type && (strcmp(cmd[i].type, "pipe") == 0 || strcmp(cmd[i].type, "or") == 0))
        {
            if(i == 0 || i == (len_cmd - 1))
                return 1;
            if(!cmd[i + 1].tab || !cmd[i - 1].tab)
                return(1);
            if(strcmp(cmd[i - 1].type, "char") != 0)
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
    while(i < len_cmd)
    {
        if((cmd[i].type && strcmp(cmd[i].type, "redir") == 0))
        {
            printf("type: %s\n", cmd[i].type);
            if(i == (len_cmd - 1))
                return 1;
            if(!cmd[i + 1].type || !cmd[i - 1].type)
                return(1);
        }
        i++;
    }
    printf("check pipe\n");
    return(0);
}
