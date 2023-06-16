#include "minishell.h"

char *dollar_status(s_cmd *cmd, char *prompt)
{
    int i;
    char *new_prompt;

    i = -1;
    new_prompt = malloc(sizeof(char) + 1);
    while(prompt[++i])
    {
        if(prompt[i] == 63)
            continue;
        if(prompt[i] == 36 && prompt[i+1] == 63)
        {
            if(!new_prompt)
                new_prompt = alloc_one(cmd->exitstatus + '0');
            else
                new_prompt = join_one(new_prompt, cmd->exitstatus + '0');
        }
        else
        {
            if(!new_prompt)
                new_prompt = alloc_one(prompt[i]);
            else
                new_prompt = join_one(new_prompt, prompt[i]);
        }
    }
    new_prompt[i] = '\0';
    return(new_prompt);
}

void expand_status(s_cmd *prompt)
{
    int i;

    i = -1;
    while(++i < prompt->nb_tabs)
    {
        if(prompt->cmd[i].tab && strchr(prompt->cmd[i].tab, '$'))
            prompt->cmd[i].tab = dollar_status(prompt, prompt->cmd[i].tab);
    }
}

char  *expand_prompt(char *prompt)
{
    int i;
    char *new_prompt;

    i = 0;
    new_prompt = NULL;
    // printf("prompt: %s\n", prompt);
    while(i < (int)ft_strlen(prompt))
    {
        // printf("i: %d %c\n", i, prompt[i]);
        if(prompt[i] == 39 || prompt[i] == 92)
        {
            i++;
            new_prompt = quoted_prompt(new_prompt, prompt, &i, 39);
        }
        else if(prompt[i] == 34)
            new_prompt = dquoted_prompt(new_prompt, prompt, &i);
        else if(prompt[i] == 36 &&  prompt[i + 1] != 64 && prompt[i + 1] != 61 && prompt[i + 1] != 63)
            new_prompt = dollar_prompt(new_prompt, prompt, &i);
        else
        {
            if(prompt[i + 1] == 64)
                i += 2;
            if(!new_prompt)
                new_prompt = alloc_one(prompt[i]);
            else
                new_prompt = join_one(new_prompt, prompt[i]);
        }
        i++;
    }
    // printf("new_prompt: %s\n", new_prompt);
    return(new_prompt);
}

void expand_cmd(s_cmd *prompt)
{
    int i;

    i = 0;
    while(i < prompt->nb_tabs && prompt->cmd[i].type)
    {
        // prompt->cmd[i].tab = expand_prompt(prompt->cmd[i].tab);
        // prompt->cmd[i].tab = wildcard_expand(prompt->cmd[i].tab, prompt);
        printf("cmd[%d]: %s type: %s\n", i, prompt->cmd[i].tab, prompt->cmd[i].type);
        i++;
    }
    prompt->nb_tabs = i;
}