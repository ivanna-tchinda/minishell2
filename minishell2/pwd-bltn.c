#include "minishell.h"

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melissaadjogoua <marvin@42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 15:24:25 by melissaadjogo     #+#    #+#             */
/*   Updated: 2023/06/05 15:51:17 by melissaadjogo    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ssize_t find_good_env(const char *env_param) 
{
    if (!var_envir || !env_param)
        return (-1);

    size_t len = strlen(env_param);
    size_t i = 0;

    while (var_envir[i++]) 
    {
        if (strncmp(var_envir[i], env_param, len) == 0 && var_envir[i][len] == '=') 
        {    
            return (i);
        }
    }
    return (-1);
}

char *recup_env(const char *env_param) 
{
    ssize_t i = find_good_env(env_param);

    if (i != -1) 
    {
        return (strdup(var_envir[i] + strlen(env_param) + 1));
    }
    return (NULL);
}

int	pwd_bltn()
{   
	char    *path;

	path = recup_env("PWD");
    printf("%s\n", path);
    return (0);
}
