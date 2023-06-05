#include "minishell.h"

char *get_var(char *new_var)
{
    (void)new_var;
    printf("new_var: %s\n", new_var);

    return(NULL);
}

void export_bltn(char *new_var)
{
    int prev_len;
    int i;
    char **env_temp;
    char *nvar;

    nvar = get_var(new_var);
    (void)nvar;
    prev_len = -1;
    i = -1;
    while(envir[++prev_len]);
    env_temp = (char **)malloc(sizeof(char *) * prev_len + 1);
    while(envir[++i])
    {
        env_temp[i] = envir[i];
        envir[i] = NULL;
    }
    env_temp[i] = nvar;
    envir = env_temp;
}