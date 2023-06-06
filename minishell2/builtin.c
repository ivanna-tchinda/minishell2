#include "minishell.h"

int around_bltn(char *tab, char *bltn, int j)
{
    if(tab[j + 1] && (tab[j + 1] != 34 && tab[j + 1] != 39 && tab[j+1] != 32))
        return(1);
    while(tab[--j] != bltn[0]);
    j--;
    if(tab[j - 1] && (tab[j - 1] != 34 || tab[j - 1] != 39 || tab[j-1] != 32))
        return(1);
    return(0);
}

int check_command(char *tab, char *bltn) //regarde s'il y a un pb a cote du bltn
{
    int i;
    int j;

    i = 0;
    j = 0;
    while(tab[i])
    {
        j = 0;
        if(tab[i] == bltn[j])
        {
            while(tab[i] && tab[++i] == bltn[++j]);
            if(j == (int)ft_strlen(bltn))
            {
                if(around_bltn(tab, bltn, i-1))
                {
                    printf("problem around bltn\n");
                    return(0);
                }
            }
        }
        i++;
    }
    return(1);
}

int built_intab(char *tab)
{
    char *tmptab;

    tmptab = tab;
    while(*tmptab)
    {
        if(strncmp(tmptab, "echo", 4) == 0)
            return(check_command(tab, "echo"));
        if(strncmp(tmptab, "cd", 2) == 0)
            return(check_command(tab, "cd"));
        if(strncmp(tmptab, "pwd", 3) == 0)
            return(check_command(tab, "pwd"));
        if(strncmp(tmptab, "export", 6) == 0)
            return(check_command(tab, "export"));
        if(strncmp(tmptab, "unset", 5) == 0)
            return(check_command(tab, "unset"));
        if(strncmp(tmptab, "env", 3) == 0)
            return(check_command(tab, "env"));
        if(strncmp(tmptab, "exit", 4) == 0)
            return(check_command(tab, "exit"));
        tmptab++;
    }
    return(0);
}

int is_builtin(char *cmd)
{
    if(built_intab(cmd) == 1)
        return(1);
    return(0);
}

void exec_bltn(char *cmd)
{
    // char **envir;
    if(strncmp(cmd, "echo", 4) == 0)
        echo_bltn(cmd);
    else if(strncmp(cmd, "cd", 2) == 0)
        printf("cd\n");
    else if(strncmp(cmd, "pwd", 3) == 0)
        pwd_bltn();
    else if(strncmp(cmd, "export", 6) == 0)
        export_bltn(cmd);
    else if(strncmp(cmd, "unset", 5) == 0)
        printf("unset\n");
    else if(strncmp(cmd, "env", 3) == 0)
        env_bltn();
    else if(strncmp(cmd, "exit", 4) == 0)
        printf("exit\n");
}