#include "minishell.h"

int check_correct_exit(char **args)
{
    int i = 1;
    int num_args = 1;
    while (args[i])
    {
        num_args++;
        i++;
    }
    if (num_args > 2)
    {
        printf("exit: too many arguments\n");
        return 1;
    }
    char *arg = args[1];
    int j = 0;
    while (arg[j] != '\0')
    {
        if (isalpha(arg[j]))
        {
            printf("invalid exit code\n");
            return 1;
        }
        j++;
    }
    return 0;
}

int check_exit_signs(char **args)
{
    char *arg = args[1];
    int i = 0;
    int count = 0;

    while (arg[i] != '\0')
    {
        if (arg[i] == '+' || arg[i] == '-')
        {
            count = 1;
            while (arg[i + 1] == arg[i])
            {
                count++;
                i++;
            }
            if (count > 2)
            {
                printf("Invalid exit code\n");
                return (1);
            }
        }
        i++;
    }
    return 0;
}

int execute_exit(char **args) 
{
        if (args[1] != NULL) 
        {
            // Si un argument est présent, convertissez-le en un nombre entier	
			if (check_correct_exit(args) == 1 || 
			check_exit_signs(args) == 1)
				exit(-1);

            long long exit_code = atoll(args[1]);
			if (strcmp(args[1], "9223372036854775808") == 0 || 
				strcmp(args[1], "-9223372036854775808") == 0)
			{
					printf("exit: %s numeric argument required\n", args[1]);
					exit(0);
			}
	else 
		   {
			 	printf("exit 💙\n");
            	exit(exit_code);
		   }
        } 
    else 
        {
            // Sinon, sortir avec le code de sortie par défaut (0)
            printf("exit 💚\n");
            exit(0);
        }
     return (1);  
}

void exit_bltn(char *prompt)
{
    char **exit_cmd;

    exit_cmd = ft_split(prompt, ' ');
    execute_exit(exit_cmd);
}