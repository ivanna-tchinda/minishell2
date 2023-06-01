#include "minishell.h"

char *alloc_one(char c)
{
    char *new_alloc;

    new_alloc = malloc(sizeof(char) + 1);
    new_alloc[0] = c;
    new_alloc[1] = '\0';
    return(new_alloc);
}

char *join_one(char *str, char c)
{
    char *new_str;
    int i;

    new_str = malloc(sizeof(char) * ft_strlen(str) + 2);
    i = 0;
    while(i < (int)ft_strlen(str))
    {
        new_str[i] = str[i];
        i++;
    }
    new_str[i] = c;
    new_str[i + 1] = '\0';
    return(new_str);
}


char *quoted_prompt(char *new_prompt, char *prompt, int *i, int quote_type)
{
    int j;
    char *new_tab;
    char *ret;

    ret = NULL;
    printf("i: %d\n", *i);
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
    {
        ret = ft_strjoin(new_prompt, new_tab);
        printf("ret j: %s %d\n", ret, *i);
    }
    else
    {
        ret = new_tab;
        printf("ret nt: %s\n", ret);
    }
    return(ret);
}

char *before_equal(char *envp)
{
    int i;

    i = -1;
    while(envp[++i] && envp[i] != '=');
    return(ft_substr(envp, 0, i));

}

char *findenv(char *varenv, char **envp)
{
    int i;
    char *beforeq;
    (void)beforeq;

    i = 0;
    while(envp[i])
    {
        beforeq = before_equal(envp[i]);
        if(strncmp(beforeq, varenv, ft_strlen(beforeq)) == 0)
            return(envp[i] + ft_strlen(beforeq) + 1);
        i++;
    }
    return(NULL);
}

char *dollar_sign(char *new_prompt, char *prompt, char *envp[], int *i)
{
    int j;
    char *varenv;
    char *goodenv;

    j = *i;
    printf("prompt: %s\n", prompt);
    if(prompt[*i -1] == 92 || !prompt[*i + 1])
    {
        printf("new: %s\n", new_prompt);
        return(new_prompt);
    }
    while(prompt[*i] != 34 && prompt[*i] != 39 && prompt[*i] && prompt[*i + 1] != 36) //!="" && existe && +1!=$
        (*i)++;
    varenv = ft_substr(prompt, j + 1, (*i) - j - 1);
    if(!varenv)
        return(new_prompt);
    goodenv = findenv(varenv, envp);
    if(!new_prompt && goodenv)
        return(goodenv);
    if(!goodenv)
    {
        printf("newp: %s\n", new_prompt);
        return(new_prompt);
    }
    return(ft_strjoin(new_prompt, goodenv));
}


char *dquoted_prompt(char *new_prompt, char *prompt, int *i, char *envp[])
{
    (void)envp;
    while(prompt[*i])
    {
        if(prompt[*i] == 36 && prompt[*i - 1] != 92
            && prompt[*i + 1] != 64 && prompt[*i + 1] != 34 && prompt[*i + 1] != 39) //$ && -1!=\ && +1!=@
            new_prompt = dollar_sign(new_prompt, prompt, envp, i);
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
    }
    return(new_prompt);
}

char *dollar_prompt(char *new_prompt, char *prompt, int *i, char *envp[])
{
    while(prompt[*i])
    {
        if(prompt[*i] == 36) //$
        {
            new_prompt = dollar_sign(new_prompt, prompt, envp, i);
            printf("in dollar %s\n", new_prompt);
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
                {
                    new_prompt = join_one(new_prompt, prompt[*i - 1]);
                    printf("in join1 %s\n", new_prompt);

                }
                if(prompt[*i] != 34 && prompt[*i] != 39)
                    new_prompt = join_one(new_prompt, prompt[*i]);
                printf("in join2 %s\n", new_prompt);
            }
        }
        (*i)++;
    }
    return(new_prompt);
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
            printf("quote %s\n", new_prompt);
            i++;
            new_prompt = quoted_prompt(new_prompt, prompt, &i, 39);
        }
        else if(prompt[i] == 34)
        {
            printf("dquote %s\n", new_prompt);
            new_prompt = dquoted_prompt(new_prompt, prompt, &i, envp);
        }
        else if(prompt[i] == 36 &&  prompt[i + 1] != 64)
        {
            printf("dollar %s\n", new_prompt);
            new_prompt = dollar_prompt(new_prompt, prompt, &i, envp);
        }
        else
        {
            if(prompt[i + 1] == 64)
                i += 2;
            if(!new_prompt)
                new_prompt = alloc_one(prompt[i]);
            else
            {
                printf("here %s\n", new_prompt);
                new_prompt = join_one(new_prompt, prompt[i]);
            }
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