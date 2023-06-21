#include "minishell.h"

int ft_execpar(s_cmd *prompt, int *i)
{
	int ret = 0;
	ret = exec_par(prompt->cmd[*i].tab, prompt, 0, i);
	(*i) += 2;
	if(*i >= prompt->nb_tabs)
		return ret;
	if(!strcmp(prompt->cmd[*i - 1].type, "or") && ret)
		ft_execpar(prompt, i);
	else if(!strcmp(prompt->cmd[*i - 1].type, "or"))
		(*i) +=2;
	if(*i >= prompt->nb_tabs)
		return ret;
	if(ret)
		return ret;
	if(!strcmp(prompt->cmd[*i - 1].type, "and"))
		ft_execpar(prompt, i);
	return(ret);
}

void ft_parentheses(s_cmd *prompt, int *i, s_token *token, s_cmd *par_prompt)
{
	int ret = 0;
	int j;

	j = *i;
	(void)ret;
	prompt->cmd[j].tab = ft_substr(prompt->cmd[j].tab, 1, ft_strlen(prompt->cmd[j].tab) - 2);
	free_token(token);
	attribute_types(token, prompt->cmd[j].tab);
	par_prompt->nb_tabs = tab_of_cmd(par_prompt, token);
	par_prompt->nb_cmd = only_cmd(par_prompt);
	ret = ft_execpar(par_prompt, &j);
	// (*i)++;
	// printf("i: %d\n", *i);
	if(*i + 1 < prompt->nb_tabs)
	{
		if(!strcmp(prompt->cmd[*i + 1].type, "and"))
			ft_and(prompt, i, ret);
		else if(!strcmp(prompt->cmd[*i + 1].type, "or"))
			or_cmd(ret, prompt, i);
	}
	return;
}
