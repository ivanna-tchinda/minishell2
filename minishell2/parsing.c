#include "minishell.h"

char *substr_cmd(s_token *token, int start, int end)
{
    char *cmd;
    int i;

    cmd = malloc(sizeof(char) * end - start);
    i = 0;
    if(start == end)
    {
        // printf("token: %c\n", token[start].token);
        return(&token[start].token);
    }
    while(start < end)
    {
        cmd[i] = token[start].token;
        i++;
        start++;
    }
    cmd[i] = '\0';
    // printf("token: %s\n", cmd);
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
            // printf("3quote %c %c\n", token[*i].token, token[j].token);
            while(token[++j].token != 39);
            j++;
        }
        else if(token[j].type != token[j+1].type)
            cmd->tab = substr_cmd(token, (*i)++, j);
        j++;
    }
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
        if(strcmp(prompt->cmd[i].type, "char") != 0)
            count++;
        i++;
    }
    if(strcmp(prompt->cmd[i - 1].type, "char") == 0)
        count++;
    return(count);
}

int tab_of_cmd(s_cmd *prompt, s_token *token)
{
    int i;
    int i_tab;
    int nb_tokens;

    i = 0;
    i_tab = -1;
    nb_tokens = ft_nbtokens(token);
    // printf("nb token %d\n", nb_tokens);
    prompt->cmd = (s_info *)malloc(sizeof(s_info) * nb_tokens);
    while(++i_tab < nb_tokens)
        add_totab(&prompt->cmd[i_tab], token, &i);
    return(nb_tokens);
}

void attribute_types(s_token *token, char *line)
{
    int i;

    i = -1;
    while(line[++i])
    {
        if(line[i] == 124)
            token[i].type = "pipe";
        else if(line[i] == 60 || line[i] == 62)
            token[i].type = "redir";
        else
            token[i].type = "char";
        token[i].token = line[i];
        // printf("att_type token %d: %c\n", i, token[i].token);
    }
}

int unclosed_quote(char *line)
{
    int i;

    i = -1;
    while(line[++i])
    {
        if(line[i] == 34)
        {
            while(line[i] && line[++i] != 34);
            if(i == (int)ft_strlen(line))
                return(1);
        }
        else if(line[i] == 39)
        {
            while(line[i] && line[++i] != 39);
            if(i == (int)ft_strlen(line))
                return(1);
        }
    }
    return(0);
}

int ft_parsing(s_cmd *prompt, s_token *token, char *line)
{
    int len_cmd;
    if(unclosed_quote(line))
        return(write(1, "error: unclosed quote\n", 22), 1);
    attribute_types(token, line);
    len_cmd = tab_of_cmd(prompt, token);
    prompt->nb_tabs = len_cmd;
    prompt->nb_cmd = only_cmd(prompt);
    int i;

    i = 0;
    // printf("nb cmd: %d\n", prompt->nb_tabs);
    while(i < len_cmd)
    {
        printf("prompt->cmd[%d].tab = %s\n", i, prompt->cmd[i].tab);
        printf("prompt->cmd[%d].type = %s\n", i, prompt->cmd[i].type);
        i++;
    }
    //checker l'ordre des arguments
    if(parse_command(prompt, len_cmd))
        return (1);
    // free(prompt->cmd->tab);
    return(0);
}