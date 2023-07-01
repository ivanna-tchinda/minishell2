#include "minishell.h"

void env_bltn(void)
{
    int i;

    i = -1;
    while(var_envir[++i])
    {
        if(strchr(var_envir[i], '='))
            printf("%s\n", var_envir[i]);
    }
    printf("last %s\n", var_envir[i]);
    exit(0);
}

int after_optn(char *line)
{
    int i;
    int j;

    i = after_echo(line);
    while(line[i] == 32)
        i++;
    while(line[i] == 45)//-
    {
        j = i;
        while(line[++i] != 32 && line[i])
        {
            if(line[i] != 'n' && line[i] && line[i] != 32)
                return(j);
        }
        i++;
    }
    while(line[i] == 32)
        i++;
    return(i);
}

int after_echo(char *line)
{
    int i;
    int j;
    char *echo;

    i = 0;
    j = 0;
    echo = "echo";
    while(line[i])
    {
        if(line[i] == echo[j])
        {
            while(line[i] == echo[j] && line[i])
            {
                i++;
                j++;
            }
            while(line[i] == 32)
                i++;
            if(j == (int)ft_strlen(echo))
                return(i);
        }
        i++;
    }
    return(i);
}

int is_optn(char *line)
{
    int i;

    i = after_echo(line);
    while(line[i] == 32)
        i++;
    if(line[i] == 45)
    {
        while(line[++i] != 32 && line[i]) //option
        {
            if(line[i] && line[i] != 'n' && line[i] != 32) //mauvaise option
                return(1);
        }
        return(0);
    }
    return(1);

}

void echo_bltn(char *line)
{
    int i;

    if(is_optn(line))
        i = after_echo(line);
    else
        i = after_optn(line);
    if(i == 0)
        return;
    while(line[i] && i < (int)ft_strlen(line))
    {
        if(line[i] == 34)
        {
            i++;
            while(line[i] != 34 && line[i])
            {
                printf("%c", line[i]);
                i++;
            }
            continue;
        }
        if(line[i] == 39)
        {
            i++;
            while(line[i] != 39 && line[i])
            {
                printf("%c", line[i]);
                i++;
            }
            continue;
        }
        if(line[i] == 32 && (line[i + 1] == 32 || !line[i + 1]))
        {
            i++;
            continue;
        }
        printf("%c", line[i]);
        i++;
    }
    if(is_optn(line))
        printf("\n");
    exit(1);
}