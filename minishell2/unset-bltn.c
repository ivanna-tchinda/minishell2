#include "minishell.h"

char *get_unsetvar(char *cmd)
{
    int i;
    int len;
    int j;
    int deb;
    char *new_tab;

    i = 0;
    j = 0;
    len = 0;
    while(cmd[i] && cmd[i] != 32)
        i++;
    deb = i + 1;
    while(cmd[++i])
        len++;
    new_tab = malloc(sizeof(char) * len + 1);
    while(j < len)
    {
        new_tab[j] = cmd[deb];
        deb++;
        j++;
    }
    new_tab[j] = '\0';
    return(new_tab);

}

int equal_match(char *nvar)
{
    int i;

    i = -1;
    while(nvar[++i])
    {
        if(nvar[i] == '=')
            return (1);
    }
    return (0);
}

void unset_bltn(char *cmd)
{
    int new_len;
    int i;
    int itab;
    char *nvar;
    char **env_temp;

    new_len = -1;
    i = -1;
    itab = -1;
    nvar = get_unsetvar(cmd);
    while(var_envir[++new_len]);
    env_temp = (char **)malloc(sizeof(char *) * new_len);
    while(var_envir[++i])
    {
        if(strncmp(var_envir[i], nvar, ft_strlen(nvar)) || equal_match(nvar))
            env_temp[++itab] = ft_strdup(var_envir[i]);
        free(var_envir[i]);
    }
    env_temp[i] = NULL;
    set_envir(env_temp);
}