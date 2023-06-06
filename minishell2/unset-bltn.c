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

void unset_bltn(char *cmd)
{
    int new_len;
    int i;
    char *nvar;
    char **env_temp;

    new_len = -1;
    i = -1;
    nvar = get_unsetvar(cmd);
    while(envir[++new_len]);
    env_temp = (char **)malloc(sizeof(char *) * new_len);
    while(envir[++i])
    {
        if(strncmp(envir[i], nvar, ft_strlen(before_equal(envir[i]))))
            env_temp[i] = ft_strdup(envir[i]);
        free(envir[i]);
    }
    env_temp[i] = NULL;
    set_envir(env_temp);
}