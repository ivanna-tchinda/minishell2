#include "minishell.h"

char *one_asterisk(char *prompt, s_cmd *cmd) // * || *.c || a*
{
    if(ft_strlen(prompt) == 1) // *
        return(allfiles_wcd(cmd));
    else if(prompt[0] != 42) // a*
        return(before_wcd(prompt, cmd));
    else if(prompt[0] == 42)
        return(after_wcd(prompt, cmd)); // *.c
    return(prompt);
}

char *two_asterisk(char *prompt, s_cmd *cmd) // a*o* || *o* || *o*.c || a*o*.c
{
    (void)cmd;
    if(prompt[0] == 42 && prompt[ft_strlen(prompt) - 1] == 42) // *o*
        return(between_wcd(prompt, cmd));
    else if(prompt[0] == 42) // *o*.c
        return(aftertwo_wcd(prompt, cmd));
    else if(prompt[ft_strlen(prompt) - 1] == 42) //a*o*
        return(beforetwo_wcd(prompt, cmd));
    return(alltwo_wcd(prompt, cmd)); //a*o*.c
}

char *modify_wildcard(char *prompt, s_cmd *cmd)
{
    int i;
    int count_w;

    i = -1;
    count_w = 0;
    while(prompt[++i])
    {
        if(prompt[i] == 42)
            count_w++;
        else if(prompt[i] == 34)
            while(prompt[++i] && prompt[i] != 34);
        else if(prompt[i] == 39)
            while(prompt[++i] && prompt[++i] != 39);
    }
    if(count_w == 0)
        return(prompt);
    if(count_w == 1)
        return(one_asterisk(prompt, cmd));
    return(two_asterisk(prompt, cmd));

}

char *wildcard_prompt(char *prompt, s_cmd *cmd)
{
    char **tab_prompt;
    char *new_prompt;
    int i;

    i = -1;
    tab_prompt = ft_split(prompt, ' ');
    while(tab_prompt[++i])
    {
        if(ft_strchr(tab_prompt[i], '*'))
            tab_prompt[i] = modify_wildcard(tab_prompt[i], cmd);
        // printf("tab:%s\n", tab_prompt[i]);
    }
    new_prompt = tabtochar(tab_prompt);
    // printf("np:%s\n", new_prompt);
    return(new_prompt);
}

int wcd_inquote(char *prompt)
{
    int i;

    i = 0;
    while(prompt[i])
    {
        if(prompt[i] == 34)
        {
            while(prompt[++i] != 34)
            {
                if(prompt[i] == '*')
                    return 1;
            }
        }
        i++;
    }
    i = 0;
    while(prompt[i])
    {
        if(prompt[i] == 39)
        {
            while(prompt[++i] != 39)
            {
                if(prompt[i] == '*')
                    return 1;
            }
        }
        i++;
    }
    return(0);
}

char *wildcard_expand(char *prompt, s_cmd *cmd)
{
    if(prompt && ft_strchr(prompt, '*'))
        return(wildcard_prompt(prompt, cmd));
    return(prompt);
}