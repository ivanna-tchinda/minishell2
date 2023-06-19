#include "minishell.h"

void ft_parentheses(s_cmd *prompt, int *i, s_token *token, s_cmd *par_prompt)
{
	prompt->cmd[*i].tab = ft_substr(prompt->cmd[*i].tab, 1, ft_strlen(prompt->cmd[*i].tab) - 2);
	free_token(token);
	attribute_types(token, prompt->cmd[*i].tab);
	par_prompt->nb_tabs = tab_of_cmd(par_prompt, token);
	par_prompt->nb_cmd = only_cmd(par_prompt);
	ft_exec(par_prompt, token);
	(*i)++;
	if(!strcmp(prompt->cmd[*i].type, "and"))
		ft_and(prompt, i, 0);
	else if(!strcmp(prompt->cmd[*i].tab, "or"))
		or_cmd(0, prompt, i);
	return;
}
