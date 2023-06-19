#include "minishell.h"

// void ft_andpar(s_cmd *prompt, int *i, int ret)
// {
// 	int ret= 0;
// 	*i += 2;
// 	// ret = exec_lastcmddoc(prompt->cmd[*i].tab, prompt, 0, NULL, i);
// 	if(ret)
// 		printf("stop\n");
	
// }

int ft_execpar(s_cmd *prompt, int *i)
{
	int ret = 0;
	ret = exec_par(prompt->cmd[*i].tab, prompt, 0, i);
	(*i) += 2;
	if(*i >= prompt->nb_tabs)
		return ret;
	if(!strcmp(prompt->cmd[*i - 1].type, "or") && ret)
		ft_execpar(prompt, i);
	if(ret)
		return ret;
	if(!strcmp(prompt->cmd[*i - 1].type, "and"))
		ft_execpar(prompt, i);
	return(ret);
}

void ft_parentheses(s_cmd *prompt, int *i, s_token *token, s_cmd *par_prompt)
{
	int ret = 0;
	(void)ret;
	prompt->cmd[*i].tab = ft_substr(prompt->cmd[*i].tab, 1, ft_strlen(prompt->cmd[*i].tab) - 2);
	free_token(token);
	attribute_types(token, prompt->cmd[*i].tab);
	par_prompt->nb_tabs = tab_of_cmd(par_prompt, token);
	par_prompt->nb_cmd = only_cmd(par_prompt);
	ft_execpar(par_prompt, i);
	return;
}
