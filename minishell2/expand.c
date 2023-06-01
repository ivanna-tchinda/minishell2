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
        //printf("new_prompt: %s\n", new_prompt);
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
        printf("expanded status: %s\n", prompt->cmd[i].tab);
    }
}

char  *expand_prompt(char *prompt, char *envp[])
{
    int i;
    char *new_prompt;

    i = 0;
    new_prompt = NULL;
    while(i < (int)ft_strlen(prompt))
    {
        if(prompt[i] == 39 || prompt[i] == 92)
        {
            i++;
            new_prompt = quoted_prompt(new_prompt, prompt, &i, 39);
        }
        else if(prompt[i] == 34)
            new_prompt = dquoted_prompt(new_prompt, prompt, &i, envp);
        else if(prompt[i] == 36 &&  prompt[i + 1] != 64 && prompt[i + 1] != 61 && prompt[i + 1] != 63)
            new_prompt = dollar_prompt(new_prompt, prompt, &i, envp);
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
    return(new_prompt);
}

void expand_cmd(s_cmd *prompt, char *envp[])
{
    int i;

    i = -1;
    while(++i < prompt->nb_tabs)
    {
        prompt->cmd[i].tab = expand_prompt(prompt->cmd[i].tab, envp);
        printf("expanded cmd: %s\n", prompt->cmd[i].tab);
    }
    
}