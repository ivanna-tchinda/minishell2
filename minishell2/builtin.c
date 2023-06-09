#include "minishell.h"

int around_bltn(char *tab, char *bltn, int j)
{
    if(tab[j + 1] && tab[j+1] != 32)
    {
        while(tab[j] != bltn[0])
            j--;
        if(!(tab[j - 1] && tab[j - 1] != 34 && tab[j - 1] != 39))
            return(1);
    }
    while(tab[j] != bltn[0])
        j--;
    if(tab[j - 1] && tab[j - 1] != 34 && tab[j - 1] != 39 && tab[j-1] != 32)
        return(1);//pb
    return(0);//ok
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
            while(bltn[j] && tab[++i] == bltn[++j]);
                // printf("same %c:%c\n", tab[i], bltn[j]);
            // printf("j: %d strlen: %ld\n", j, ft_strlen(bltn));
            if(j == (int)ft_strlen(bltn))
            {
                if(around_bltn(tab, bltn, i-1))
                    return(0);//pb
            }
        }
        i++;
    }
    return(1);//ok
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
    if(built_intab(cmd) == 1) //ok
        return(1);
    return(0);//pb
}

void exec_bltn(char *cmd, s_cmd *prompt)
{
    (void)prompt;
    if(strstr(cmd, "echo"))
        echo_bltn(cmd);
    else if(!strncmp(cmd, "cd", 2))
        cd_bltn(cmd);
    else if(!strncmp(cmd, "pwd", 3))
        pwd_bltn();
    else if(!strncmp(cmd, "export", 6))
        export_bltn(cmd);
    else if(!strncmp(cmd, "unset", 5))
        unset_bltn(cmd);
    else if(!strncmp(cmd, "env", 3))
        env_bltn();
    else if(!strncmp(cmd, "exit", 4))
        exit_bltn(cmd);
    
}