#include "minishell.h"

int around_bltn(char *tab, char *bltn, int j)
{
    if(tab[j + 1] && (tab[j + 1] != 34 || tab[j + 1] != 39 || tab[j+1] != 32))
        return(1);
    while(--j > (int)ft_strlen(bltn));
    j--;
    if(tab[j - 1] && (tab[j - 1] != 34 || tab[j - 1] != 39 || tab[j-1] != 32))
    {
        printf("%c\n", tab[j]);
        return(1);
    }
    return(0);
}

int check_command(char *tab, char *bltn)
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
            while(tab[++i] == bltn[++j] && tab[i]);
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
    // printf("no problem around bltn\n");
    return(1);
}

int built_intab(char *tab)
{
    char *tmptab;

    tmptab = tab;
    // tab = unquote_tab(tab);
    while(*tmptab)
    {
        printf("tab: %s\n", tmptab);
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

int is_builtin(s_cmd *prompt)
{
    int i;

    i = 0;
    while(i < prompt->nb_tabs)
    {
        if(built_intab(prompt->cmd[i].tab) == 1)
        {
            printf("there is a bltin %s\n", prompt->cmd[i].tab);
            return(1);
        }
        i++;
    }
    return(0);
}