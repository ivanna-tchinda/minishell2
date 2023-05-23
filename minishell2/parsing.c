#include "minishell.h"


int error_characters(char *lineav)
{
    int i;
    char *line;
    int count_quote;

    i = 0;
    count_quote = 0;
    line = lineav;
    if(!line)
        return 1;
    while(line[i])
    {
        if(line[i] == 39 || line[i] == 34)
            count_quote++;
        if(line[i] == 59 || line[i] == 92)
        {
            perror("minishell: input has a forbidden character");
            return (1);
        }
        i++;
    }
    if(count_quote % 2 != 0)
    {
        perror("minishell: quote is not closed");
        return 1;
    }
    return 0;
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
        else if(line[i] == 60 || line[i] == 62)
            token[i].type = "redir";
        else if(line[i] == 32)
            token[i].type = "space";
        else if(line[i] == 36)
            token[i].type = "dollar";
        else if(line[i] == 34 || line[i] == 39)
            token[i].type = "quote";
        else if(line[i] == 38 && (line[i + 1] == 38 || line[i - 1] == 38))
            token[i].type = "and";
        else
            token[i].type = "char";
        token[i].token = line[i];
        printf("token:%c type:%s\n", token[i].token, token[i].type);
    }
}


int ft_nbtokens(s_token *token)
{
    int i;
    int count;

    i = 1;
    count = 1;
    while(token[i].token)
    {
        if(token[i].type != token[i - 1].type)
            count++;
        i++;
    }
    printf("%d\n", count);
    return(count);
}

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
    printf("token: %s\n", cmd);
    return(cmd);
}

/*int tab_of_cmd(s_cmd *prompt, s_token *token)
{
    int i;
    int j;
    int join;
    
    prompt->cmd = (s_info *)malloc(sizeof(s_info) * ft_nbtokens(token));
    i = 0;
    j = 1;
    join = 0;
    while(token[i].token)
    {
        prompt->cmd[join].type = token[i].type;
        if(token[j].type == token[j-1].type)
        {
            while(token[j].type == token[j-1].type && token[j++].type);
            prompt->cmd[join++].tab = substr_cmd(token, i, j);
            i = j;
        }
        else
        {
            if(token[j].type != token[j+1].type)
                prompt->cmd[join++].tab = substr_cmd(token, i++, j);
            j++;
        }
    }
    return(prompt->cmd[join].tab = NULL, ft_nbtokens(token));
}*/

void add_totab(s_info cmd, s_token *token, int *i)
{

}

int tab_of_command(s_cmd *prompt, s_token *token)
{
    int i;
    int i_tab;
    int nb_tokens;

    i = 0;
    i_tab = -1;
    nb_tokens = ft_nbtokens(token);
    prompt->cmd = (s_info *)malloc(sizeof(s_info) * nb_tokens);
    while(++i_tab < nb_tokens)
        add_totab(prompt->cmd[i_tab], token, &i);
}

int check_pipe(s_info *cmd, int len_cmd)
{
    int i;
    
    i = 0;
    while(i < len_cmd)
    {
        if(strcmp(cmd[i].type, "pipe") == 0)
        {
            if(i == 0 || i == (len_cmd - 1))
                return 1;
            if(strcmp(cmd[i + 1].type, "char") != 0)
                return(1);
            else if(strcmp(cmd[i - 1].type, "char") != 0)
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
        if(strcmp(cmd[i].type, "redir") == 0)
        {
            if(i == 0 || i == (len_cmd - 1))
                return 1;
            if(strcmp(cmd[i + 1].type, "char") != 0)
                return(1);
            else if(strcmp(cmd[i - 1].type, "char") != 0)
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

int ft_parsing(s_cmd *prompt, s_token *token, char *line)
{
    int len_cmd;
    if(error_characters(line))
        return 1;
    attribute_types(token, line);
    len_cmd = tab_of_cmd(prompt, token);
    int i;

    i = 0;
    while(prompt->cmd[i].tab)
    {
        printf("prompt->cmd[%d].tab = %s\n", i, prompt->cmd[i].tab);
        printf("prompt->cmd[%d].type = %s\n", i, prompt->cmd[i].type);
        i++;
    }
    if(parse_command(prompt, len_cmd))
        return (1);
    return(0);
}