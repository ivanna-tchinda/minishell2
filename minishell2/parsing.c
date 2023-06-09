#include "minishell.h"

int tab_of_cmd(s_cmd *prompt, s_token *token)
{
    int i;
    int i_tab;
    int nb_tokens;

    i = 0;
    i_tab = -1;
    nb_tokens = ft_nbtokens(token);
    prompt->cmd = (s_info *)malloc(sizeof(s_info) * nb_tokens + 1);
    while(++i_tab < nb_tokens)
        add_totab(&prompt->cmd[i_tab], token, &i);
    return(nb_tokens);
}

void attribute_types(s_token *token, char *line)
{
    int i;

    i = -1;
    while(line[++i] && i < (int)ft_strlen(line))
    {
        if(line[i] == 40)
        {
            while(line[i] != 41)
            {
                token[i].type = "parentheses";
                token[i].token = line[i];
                i++;
            }
            token[i].type = "parentheses";
            token[i].token = line[i];
            continue;
        }
        else if(line[i] == 39)
        {
            while(line[i] != 39)
            {
                token[i].type = "char";
                token[i].token = line[i];
                i++;
            }
            token[i].type = "char";
            token[i].token = line[i];
            continue;
        }
        else if(line[i] == 34)
        {
            while(line[i] != 34)
            {
                token[i].type = "char";
                token[i].token = line[i];
                i++;
            }
            token[i].type = "char";
            token[i].token = line[i];
            continue;
        }
        else if((line[i] == 124 && (line[i + 1] == 124 || line[i - 1] == 124)) )
            token[i].type = "or";
        else if((line[i] == 124 && line[i + 1] != 124 && line[i - 1] != 124) )
            token[i].type = "pipe";
        else if((line[i] == 38 && line[i + 1] == 38) || (line[i] == 38 && line[i - 1] == 38))
            token[i].type = "and";
        else if(line[i] == 60 || line[i] == 62)
            token[i].type = "redir";
        else
            token[i].type = "char";
        token[i].token = line[i];
    }
    token[i].token = '\0';
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
        if(line[i] == 124) // |
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
    if(!strcmp(line, "!"))
        return(prompt->exitstatus = 1, 1);
    else if(unclosed_quote(line) || many_tokens(line)|| parentheses(line))
        return(prompt->exitstatus = 2, write(1, "error: parse error\n", 19), 1);
    attribute_types(token, line);
    len_cmd = tab_of_cmd(prompt, token);
    prompt->nb_tabs = len_cmd;
    prompt->nb_cmd = only_cmd(prompt);
    if(parse_command(prompt, len_cmd, token))
        return (prompt->exitstatus = 2, 1);
    return(0);
}