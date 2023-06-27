#include "minishell.h"

char *get_var(char *new_var)
{
    int i;
    int len;
    int itab;
    char *varenv;

    i = 0;
    itab = 0;
    // printf("nv: %s\n", new_var);
    while(new_var[i] && new_var[i] != 32)
        i++;
    len = i;
    if(!isalpha(new_var[i + 1]) && new_var[i + 1] != '_')
        return(write(2, "error\n", 6), NULL);
    while(new_var[++len])
    {
        if(new_var[len] == '=')
        {
            if(new_var[len - 1] == ' ')
                return(write(2, "error\n", 6), NULL);
        }
    }
    varenv = (char *)malloc(sizeof(char) * len + 1);
    len = i + 1;
    while(new_var[len])
    {
        varenv[itab] = new_var[len];
        itab++;
        len++;
    }
    varenv[itab] = '\0';
    int j = -1;
    while(varenv[++j])
    {
        if(varenv[j] == 61)
            return(varenv);
    }
    return(NULL);
}

int var_alreadyset(char *new_var)
{
    int i;

    i = -1;
    new_var = before_equal(new_var);
    while(var_envir[++i])
    {
        if(!strncmp(var_envir[i], new_var, ft_strlen(new_var)))
            return 1;
    }
    return 0;
}

void modify_env(char *nvar)
{
    int i;

    i=-1;
    while(var_envir[++i])
    {
        if(strncmp(var_envir[i], nvar, ft_strlen(nvar)))
        {
            free(var_envir[i]);
            var_envir[i] = ft_strdup(nvar);
            return;
        }
    }
}

void export_bltn(char *new_var)
{
    int prev_len;
    int i;
    char **env_temp;
    char *nvar;
    int new_len;

    nvar = get_var(new_var);
    // printf("nvar: %s\n", nvar);
    if(!nvar)
        return;
    prev_len = -1;
    i = -1;
    while(var_envir[++prev_len]);
    if(!nvar || var_alreadyset(nvar))
        new_len = prev_len;
    else 
        new_len = prev_len + 1;
    env_temp = (char **)malloc(sizeof(char *) * new_len);
    while(var_envir[++i])
    {
        if(!strncmp(before_equal(var_envir[i]), nvar, ft_strlen((before_equal(nvar)))))
            env_temp[i] = ft_strdup(nvar);
        else
            env_temp[i] = ft_strdup(var_envir[i]);
        free(var_envir[i]);
    }
    free(var_envir);
    if(i < new_len)
        env_temp[i] = ft_strdup(nvar);
    env_temp[++i] = NULL;
    set_envir(env_temp);
}