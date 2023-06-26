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
        if(token[*i].token == 34) // "
        {
            while(token[++j].token != 34 && token[*i].token);
            j++;
        }
        else if(token[*i].token == 39) // '
        {
            while(token[++j].token != 39);
            j++;
        }
        else if(token[j].type != token[j+1].type)
            cmd->tab = substr_cmd(token, (*i)++, j);
        j++;
    }
} //+25 mais ok

int check_and(s_cmd *prompt, int len_cmd)
{
    int i;

    i = -1;
    while(++i < len_cmd && prompt->cmd[i].type)
    {
        if(prompt->cmd[i].type && strcmp(prompt->cmd[i].type, "and") == 0)
        {
            if(i == 0)
                return 1;
            if(strcmp(prompt->cmd[i - 1].type, "char") != 0 && strcmp(prompt->cmd[i - 1].type, "parentheses"))
                    return(1);
        } 
    }
    return 0;
}

char *remove_parentheses(char *prompt)
{
    int i;
    int j;
    char *no_par;

    i = 1;
    j = 0;
    no_par = malloc(sizeof(char) * (int)ft_strlen(prompt) - 1);
    while(i < (int)ft_strlen(prompt) - 1)
    {
        no_par[j] = prompt[i];
        j++;
        i++;
    }
    no_par[j] = '\0';
    return(no_par);
}

int parse_parentheses(s_cmd *prompt, int *i)
{
    int j;

    (*i)--;
    j = ft_strlen(prompt->cmd[*i].tab) - 1;
    while(prompt->cmd[*i].tab[--j] == 32);
    if(!isalpha(prompt->cmd[*i].tab[j]) && (prompt->cmd[*i + 1].type
        && (strcmp(prompt->cmd[*i + 1].type, "and") && strcmp(prompt->cmd[*i + 1].type, "or") && strcmp(prompt->cmd[*i + 1].type, "parentheses"))))
        return(write(2, "error: unexpected token before ')'\n", 35));
    return(0);
}

int multiple_par(char *cmd)
{
    int i;
    int count;

    i = 0;
    count = 0;
    while(i < (int)ft_strlen(cmd))
    {
        if(cmd[i] && (cmd[i] == 40 || cmd[i] == 41))
            count++;
        i++;
    }
    if(count > 2)
        return (1);
    return 0;
}

int check_parentheses(s_cmd *prompt)
{
    int i;

    i = 0;
    while(i < prompt->nb_tabs && prompt->cmd[i].type)
    {
        if(!strcmp(prompt->cmd[i].type, "parentheses"))
        {
            i++;
            if(multiple_par(prompt->cmd[i - 1].tab))
                prompt->cmd[i - 1].tab = remove_parentheses(prompt->cmd[i - 1].tab);
            else if(strchr(prompt->cmd[i - 1].tab, '|') && strncmp(strchr(prompt->cmd[i - 1].tab, '|'), "||", 2))
                prompt->cmd[i - 1].tab = remove_parentheses(prompt->cmd[i - 1].tab);
            else if(strchr(prompt->cmd[i - 1].tab, '>') || strchr(prompt->cmd[i - 1].tab, '<'))
                prompt->cmd[i - 1].tab = remove_parentheses(prompt->cmd[i - 1].tab);
            else if(prompt->cmd[i].type && strcmp(prompt->cmd[i].type, "and") && strcmp(prompt->cmd[i].type, "or") && strcmp(prompt->cmd[i].type, "char"))
                prompt->cmd[i - 1].tab = remove_parentheses(prompt->cmd[i - 1].tab);
            else if((i - 2) > -1 && strcmp(prompt->cmd[i - 2].type, "and") && strcmp(prompt->cmd[i - 2].type, "or") && strcmp(prompt->cmd[i - 2].type, "char"))
                prompt->cmd[i - 1].tab = remove_parentheses(prompt->cmd[i - 1].tab);
            else if (parse_parentheses(prompt, &i))
                return (1);
        }
        i++;
    }
    return(0);
}

void split_parentheses(s_cmd *prompt, s_token *token)
{
    int i;
    // int j;
    // int i_line;
    int count;
    char *new_line;

    i = 0;
    // j = 0;
    // i_line = 0;
    count = 0;
    while(prompt->cmd[i].type && i < prompt->nb_tabs)
    {
        count += ft_strlen(prompt->cmd[i].tab);
        i++;
    }
    new_line = NULL;
    i = 0;
    while(prompt->cmd[i].type && i < prompt->nb_tabs)
    {
        new_line = ft_strjoin(new_line, prompt->cmd[i].tab);
        i++;
    }
    free_token(token);
    attribute_types(token, new_line);
    prompt->nb_tabs = tab_of_cmd(prompt, token);
}

int check_cmd(s_cmd *prompt)
{
    int i;

    i = 0;
    while(prompt->cmd[i].type && i < prompt->nb_tabs)
    {
        if(!strcmp(prompt->cmd[i].type, "char") && i+1 < prompt->nb_cmd)
        {
            if(prompt->cmd[i + 1].type && (strchr(prompt->cmd[i + 1].tab, '<') || !strcmp(prompt->cmd[i + 1].type, "parentheses")))
                return (1);
        }
        i++;
    }
    return (0);
}

int parse_command(s_cmd *prompt, int len_cmd, s_token *token)
{
    int i;

    i = 0;
    if(len_cmd == 0)
        return (1);
    if(check_parentheses(prompt))
        return(1);
    if(strchr(prompt->cmd[i].tab, '('))
        split_parentheses(prompt, token);
    if(check_cmd(prompt))
        return(write(1, "minishell: no such file or directory\n", 37));
    while(prompt->cmd[i].type)
    {
        //cas1: pipe sans arg avant ou apres
        if(check_pipe(prompt->cmd, len_cmd))
            return(write(1, "zsh: parse error near `|'\n", 26));
        //cas2: redirection sans arg avant ou apres
        else if(check_and(prompt, len_cmd))
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
    if(!prompt->cmd[i].type)
        return(0);
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