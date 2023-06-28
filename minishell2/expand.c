#include "minishell.h"

char *dollar_status(s_cmd *cmd, char *prompt)
{
    int i;
    char *new_prompt;

    i = -1;
    new_prompt = NULL;
    while(prompt[++i])
    {
        if(prompt[i] == 63) // ?
            continue;
        if(prompt[i] == 36 && prompt[i+1] == 63)
        {
            if(!new_prompt)
                new_prompt = alloc_one(cmd->exitstatus + '0');
            else
                new_prompt = ft_strjoin(new_prompt, ft_itoa(cmd->exitstatus));
        }
        else
        {
            if(!new_prompt)
                new_prompt = alloc_one(prompt[i]);
            else
                new_prompt = join_one(new_prompt, prompt[i]);
        }
    }
    new_prompt[++i] = '\0';
    return(new_prompt);
}

void expand_status(s_cmd *prompt)
{
    int i;

    i = -1;
    while(++i < prompt->nb_tabs)
    {
        if(prompt->cmd[i].tab && strchr(prompt->cmd[i].tab, '$')&& strchr(prompt->cmd[i].tab, '?'))
            prompt->cmd[i].tab = dollar_status(prompt, prompt->cmd[i].tab);
    }
}

char  *expand_prompt(char *prompt)
{
    int i;
    char *new_prompt;

    i = 0;
    new_prompt = NULL;
    while(i < (int)ft_strlen(prompt))
    {
        if(prompt[i] == 32)
        {
            if(!new_prompt)
                new_prompt = alloc_one(prompt[i]);
            else
                new_prompt = join_one(new_prompt, prompt[i]);
            while(prompt[i] == 32)
                i++;
        }
        if((prompt[i] == 39  && (prompt[i+1] == 39 || (!isalpha(prompt[i - 1]) && !isalpha(prompt[i + 1])) || (prompt[i] == 92 && prompt[i + 1] == 36))))
        {
            i++;
            new_prompt = quoted_prompt(new_prompt, prompt, &i, 39);
        }
        else if(prompt[i] == 34 && (!prompt[i+1] || prompt[i+1] == 39 || (!isalpha(prompt[i - 1])/* && !isalpha(prompt[i + 1])*/)))
            new_prompt = dquoted_prompt(new_prompt, prompt, &i);
        else if(prompt[i] == 36 &&  prompt[i + 1] &&  prompt[i + 1] != 32 && prompt[i + 1] != 64 && prompt[i + 1] != 61 && prompt[i + 1] != 63)
            new_prompt = dollar_prompt(new_prompt, prompt, &i);
        else
        {
            if((prompt[i] == 92 && prompt[i + 1] == 36))
                i++;
            if(prompt[i + 1] == 64)
                i += 2;
            if(!new_prompt)
                new_prompt = alloc_one(prompt[i]);
            else
                new_prompt = join_one(new_prompt, prompt[i]);
        }
        i++;
    }
    // printf("np: %s\n", new_prompt);
    return(new_prompt);
}

void expand_cmd(s_cmd *prompt)
{
    int i;

    i = 0;
    expand_status(prompt);
    while(i < prompt->nb_tabs && prompt->cmd[i].type)
    {
        prompt->cmd[i].tab = wildcard_expand(prompt->cmd[i].tab, prompt);
        // printf("tab: %s\n", prompt->cmd[i].tab);
        prompt->cmd[i].tab = expand_prompt(prompt->cmd[i].tab);
        // printf("tab: %s\n", prompt->cmd[i].tab);
        // if(!ft_split(prompt->cmd[i].tab, ' ')[0])
        //     prompt->nb_cmd--;
        i++;
    }
    prompt->nb_tabs = i;
}