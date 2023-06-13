#include "minishell.h"

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

void attribute_types(s_token *token, char *line)
{
    int i;

    i = -1;
    while(line[++i])
    {
        // if((line[i] == 124 && line[i + 1] == 124) || (line[i] == 124 && line[i - 1] == 124))
        //     token[i].type = "or";
        if((line[i] == 124 && line[i + 1] != 124 && line[i - 1] != 124) )
            token[i].type = "pipe";
        else if((line[i] == 38 && line[i + 1] == 38) || (line[i] == 38 && line[i - 1] == 38))
            token[i].type = "and";
        else if(line[i] == 60 || line[i] == 62)
            token[i].type = "redir";
        else
            token[i].type = "char";
        token[i].token = line[i];
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

int many_tokens(char *line)
{
    int i;
    int count;

    i = -1;
    count = 0;
    while(line[++i])
    { 
        if(line[i] == 124)
        {
            while(line[i++] == 124)
                count++;
            if(count > 2)
                return 1;
            count = 0;
        }
        else if(line[i] == 38)
        {
            while(line[i++] == 38)
                count++;
            if(count != 2)
                return 1;
            count = 0;
        }
    }
    return 0;
}

int ft_parsing(s_cmd *prompt, s_token *token, char *line)
{
    int len_cmd;
    if(unclosed_quote(line) || many_tokens(line) || parentheses(line))
        return(write(1, "error: parse error\n", 19), 1);
    attribute_types(token, line);
    len_cmd = tab_of_cmd(prompt, token);
    prompt->nb_tabs = len_cmd;
    prompt->nb_cmd = only_cmd(prompt);
    //checker l'ordre des arguments
    if(parse_command(prompt, len_cmd))
        return (1);
    return(0);
}