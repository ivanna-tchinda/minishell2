#include "minishell.h"

char *get_var(char *new_var)
{
    int i;
    int len;
    int itab;
    char *varenv;

    i = 0;
    itab = 0;
    while(new_var[i] && new_var[i] != 32)
        i++;
    len = i;
    while(new_var[++len])
    {
        if(new_var[len] == 32)  
        {
            write(1, "error\n", 6);
            return(NULL);
        } 
    }
    varenv = (char *)malloc(sizeof(char) * len + 1);
    len = i;
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
    while(envir[++i])
    {
        if(!strncmp(envir[i], new_var, ft_strlen(new_var)))
        {
            printf("env: %s %s\n", new_var, envir[i]);
            return 1;
        }
    }
    return 0;
}

void modify_env(char *nvar)
{
    int i;

    i=-1;
    while(envir[++i])
    {
        if(strncmp(envir[i], nvar, ft_strlen(nvar)))
        {
            free(envir[i]);
            envir[i] = ft_strdup(nvar);
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

    nvar = get_var(new_var);
    if(var_alreadyset(nvar))
    {
        printf("var is already here\n");
        modify_env(nvar);
        return;
    }
    prev_len = -1;
    i = -1;
    while(envir[++prev_len]);
    if(!nvar)
        env_temp = (char **)malloc(sizeof(char *) * prev_len);
    else 
        env_temp = (char **)malloc(sizeof(char *) * prev_len + 1);
    while(envir[++i])
    {
        env_temp[i] = ft_strdup(envir[i]);
        free(envir[i]);
    }
    free(envir);
    if(nvar)
        env_temp[i] = ft_strdup(nvar);
    env_temp[++i] = NULL;
    set_envir(env_temp);
}