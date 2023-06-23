#include "minishell.h"

char *quoted_prompt(char *new_prompt, char *prompt, int *i, int quote_type)
{
    int j;
    char *new_tab;
    char *ret;

    ret = NULL;
    new_tab = malloc(sizeof(char) * (int)ft_strlen(prompt) - 1);
    j = 0;
    while(*i < (int)ft_strlen(prompt) && prompt[*i] != quote_type)
    {
        if(prompt[*i] != quote_type && prompt[*i] && prompt[*i] != 92)
        {
            new_tab[j] = prompt[*i];
            j++;
        }
        (*i)++;
    }
    new_tab[j] = '\0';
    if(new_prompt)
        ret = ft_strjoin(new_prompt, new_tab);
    else
        ret = new_tab;
    return(ret);
}

char *before_equal(char *envp)
{
    int i;

    i = -1;
    while(envp[++i] && envp[i] != '=');
    return(ft_substr(envp, 0, i));

}

char *findenv(char *varenv)
{
    int i;
    char *beforeq;

    i = 0;
    while(var_envir[i])
    {
        beforeq = before_equal(var_envir[i]);
        if(strncmp(beforeq, varenv, ft_strlen(varenv)) == 0)
            return(var_envir[i] + ft_strlen(beforeq) + 1);
        i++;
    }
    return(NULL);
}

char *dollar_sign(char *new_prompt, char *prompt, int *i)
{
    int j;
    char *varenv;
    char *goodenv;

    j = *i;
    if(prompt[*i -1] == 92 || !prompt[*i + 1]) // \, dntexist
        return(new_prompt);
    if((prompt[*i + 1] > 47 && prompt[*i + 1] < 58) || prompt[*i + 1] == 42)
    {
        (*i)++;
        return(new_prompt);
    }
    while(prompt[*i] != 32 && prompt[*i] != 34 && prompt[*i] != 39
        && prompt[*i] && prompt[*i + 1] != 36 && prompt[*i + 1] != 42 && prompt[*i + 1] != 41) //!="" && existe && +1!=$
        (*i)++;
    varenv = ft_substr(prompt, j + 1, (*i) - j - 1);
    if(!varenv)
        return(new_prompt);
    goodenv = findenv(varenv);
    if(!new_prompt && goodenv)
        return(goodenv);
    if(!goodenv)
        return((*i)--, ft_strjoin(new_prompt, ""));
    return(ft_strjoin(new_prompt, goodenv));
}


char *dquoted_prompt(char *new_prompt, char *prompt, int *i)
{
    while(prompt[*i])
    {
        if(prompt[*i] == 36 && prompt[*i - 1] != 92
            && prompt[*i + 1] != 64 && prompt[*i + 1] != 34
            && prompt[*i + 1] != 39) //$ && -1!=\ && +1!=@
            new_prompt = dollar_sign(new_prompt, prompt, i);
        else if(prompt[*i] != 34) //!= ""
        {
            if(prompt[*i] == 92 || prompt[*i] == 64) //!=\ || !=@
                (*i)++;
            if(!new_prompt)
                new_prompt = alloc_one(prompt[*i]);
            else
                new_prompt = join_one(new_prompt, prompt[*i]);
        }
        (*i)++;
        if(prompt[*i] == 34)
            break;
    }
    if(*i == 1)
        new_prompt = "\0";
    return(new_prompt);
}

char *dollar_prompt(char *new_prompt, char *prompt, int *i)
{
    while(prompt[*i])
    {
        if(prompt[*i] == 36) //$
        {
            new_prompt = dollar_sign(new_prompt, prompt, i);
            if(!new_prompt)
                (*i)++;
        }
        else if(prompt[*i])
        {
            if(!new_prompt)
                new_prompt = alloc_one(prompt[*i]);
            else if((*i) < (int)ft_strlen(prompt))
            {
                if(prompt[*i - 1] == 36)
                    new_prompt = join_one(new_prompt, prompt[*i - 1]);
                if(prompt[*i] != 34 && prompt[*i] != 39)
                    new_prompt = join_one(new_prompt, prompt[*i]);
            }
        }
        (*i)++;
    }
    return(new_prompt);
}
