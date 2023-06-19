#include "minishell.h"

void ft_parentheses(char *prompt, s_token *token, s_cmd *par_prompt)
{
	int j;

	j = 1;
	prompt = ft_substr(prompt, 1, ft_strlen(prompt) - 2);
	free_token(token);
	attribute_types(token, prompt);
	par_prompt->nb_tabs = tab_of_cmd(par_prompt, token);
	par_prompt->nb_cmd = only_cmd(par_prompt);
	ft_exec(par_prompt, token);
	return;
}
