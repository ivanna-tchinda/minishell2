#include "minishell.h"


char *without_spaces(char *cmd)
{
	char **no_space;
	char *to_ret;
	int i;

	i = 0;
	no_space = ft_split(cmd, ' ');
	while(no_space[i])
	{
		i++;
	}
	if(i > 1)
	{
		i = 0;
		to_ret = NULL;
		while(no_space[i])
		{
			to_ret = ft_strjoin(to_ret, no_space[i]);
			to_ret = ft_strjoin(to_ret, " ");
			i++;
		}
	}
	else
		return(no_space[0]);
	return(to_ret);
}

char *after_infile(char *prompt)
{
	int i;

	i = -1;
	while(prompt[++i] == 32);
	while(prompt[++i] && prompt[i] != 32);
	return(ft_substr(prompt, i, ft_strlen(prompt) - i));
}