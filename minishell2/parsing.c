#include "minishell.h"


int error_characters(char *lineav)
{
    int i;
    char *line;
    int count_quote;

    i = -1;
    count_quote = 0;
    line = lineav;
    // if(!line)
    //     return 1;
    while(line[++i])
    {
        if(line[i] == 39 || line[i] == 34)
            count_quote++;
        if(line[i] == 59 || line[i] == 92)
            return (write(2, "minishell: input has a forbidden character\n", 43));
    }
    if(count_quote % 2 != 0)
        return(write(2, "minishell: quote is not closed\n", 31));
    return 0;
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

void attribute_types(s_token *token, char *line)
{
    int i;

    i = -1;
    while(line[++i])
    {
        if(line[i] == 124 && (line[i + 1] == 124 || line[i - 1] == 124))
            token[i].type = "or";
        else if(line[i] == 124)
            token[i].type = "pipe";
        else if(line[i] == 60)
            token[i].type = "redirin";
        else if(line[i] == 62)
            token[i].type = "redirout";
        else if(line[i] == 36)
            token[i].type = "dollar";
        else if(line[i] == 32)
            token[i].type = "space";
        else if(line[i] == 34 || line[i] == 39)
            quote_type(token, line, &i);
        else if(line[i] == 38 && (line[i + 1] == 38 || line[i - 1] == 38))
            token[i].type = "and";
        else
            token[i].type = "char";
        token[i].token = line[i];
        // printf("att_type token %d: %c\n", i, token[i].token);
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
        if(token[i].token == 32)
        {
            while(token[i].token == 32)
                i++;
        }
        // printf("before token: %c type: %s\n", token[i].token, token[i - 1].type);
        if(token[i].type != token[i - 1].type)
        {
            // printf("token: %c != %c\n", token[i].token, token[i - 1].token);
            count++;
            // printf("count boucle 2 %d\n", i);
        }
        i++;
    }
    // printf("nb of types: %d\n", count);
    return(count);
}

char *substr_cmd(s_token *token, int start, int end)
{
    char *cmd;
    int i;

    cmd = malloc(sizeof(char) * end - start);
    i = 0;
    while(token[start].token == 32)
        start++;
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
        if(token[j].type != token[j+1].type)
            cmd->tab = substr_cmd(token, (*i)++, j);
        j++;
    }
}

int tab_of_cmd(s_cmd *prompt, s_token *token)
{
    int i;
    int i_tab;
    int nb_tokens;

    i = 0;
    i_tab = -1;
    nb_tokens = ft_nbtokens(token);
    prompt->cmd = (s_info *)malloc(sizeof(s_info) * nb_tokens);
    while(++i_tab < nb_tokens)
        add_totab(&prompt->cmd[i_tab], token, &i);
    return(nb_tokens);
}

int check_pipe(s_info *cmd, int len_cmd)
{
    int i;
    
    i = 0;
    // printf("nb cmd: %d\n", len_cmd);
    while(i < len_cmd)
    {
        // printf("type: %s\n", cmd[i].type);
        if(cmd[i].type && strcmp(cmd[i].type, "pipe") == 0)
        {
            if(i == 0 || i == (len_cmd - 1))
                return 1;
            if(!cmd[i + 1].tab || !cmd[i - 1].tab)
                return(1);
            if(strcmp(cmd[i - 1].type, "char") != 0
                && strcmp(cmd[i - 1].type, "noexp$") != 0
                && strcmp(cmd[i - 1].type, "noexp") != 0)
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
        if(strcmp(cmd[i].type, "redirin") == 0 || strcmp(cmd[i].type, "redirout") == 0)
        {
            // printf("%s\n", cmd->type);
            if(i == 0 || i == (len_cmd - 1))
                return 1;
            if(!cmd[i + 1].type || !cmd[i - 1].type)
                return(1);
            if(strcmp(cmd[i - 1].type, "char") != 0)
                return(1);
        }
        i++;
    }

    return(0);
}

int parse_command(s_cmd *prompt, int len_cmd)
{
    int i;

    i = 0;
    while(i < len_cmd)
    {
        //cas1: pipe sans arg avant ou apres
        if(check_pipe(prompt->cmd, len_cmd))
            return(write(1, "zsh: parse error near `|'\n", 26));
        //cas2: redirection sans arg avant ou apres
        else if(check_redif(prompt->cmd, len_cmd))
            return(write(1, "zsh: parse error\n", 17));
        // //cas3: commande qui n'existe pas
        // else if(check_cmd(prompt->cmd))
        //     return(write(1, "zsh: command not found\n", 23));
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

int ft_parsing(s_cmd *prompt, s_token *token, char *line)
{
    int len_cmd;
    if(error_characters(line))
    {
        printf("err char\n");
        return 1;
    }
    attribute_types(token, line);
    len_cmd = tab_of_cmd(prompt, token);
    prompt->nb_tabs = len_cmd;
    prompt->nb_cmd = only_cmd(prompt);
    printf("nb cmd %d\n", prompt->nb_cmd);
    // int i;

    // i = 0;
    // printf("nb cmd: %d\n", len_cmd);
    // while(i < len_cmd)
    // {
    //     printf("prompt->cmd[%d].tab = %s\n", i, prompt->cmd[i].tab);
    //     printf("prompt->cmd[%d].type = %s\n", i, prompt->cmd[i].type);
    //     i++;
    // }
    if(parse_command(prompt, len_cmd))
    {
        printf("err parse\n");
        return (1);
    }
    return(0);
}