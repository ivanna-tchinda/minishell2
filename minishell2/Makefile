NAME= minishell

SRC=	  main.c\
			parsing.c\
			parse2.c\
			parse3.c\
			utils.c\
			begin-exec.c\
			exec-functs.c\
			ft-execve.c\
			builtin.c\
			echo-bltn.c\
			pwd-bltn.c\
			expand.c\
			expand2.c\
			expand3.c\
			wildcard.c\
			wildcard2.c\
			wildcard3.c\
			export-bltn.c\
			unset-bltn.c\
			exit-bltn.c\
			parentheses.c\
			utils_pipex.c\
			utils-exec.c\

CC= cc  

OBJ_DIR= obj

CFLAGS=  -Wall -Werror -Wextra -g3 

OBJ= 	$(addprefix $(OBJ_DIR)/,$(SRC:%.c=%.o))

RM=	rm -rf

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@ 

all:		$(NAME)

$(NAME):	$(OBJ)
			$(CC) $(OBJ) -o  $(NAME) -lreadline -lncurses -ltinfo -ltermcap 

clean:		
		$(RM) $(OBJ)
		$(RM) $(OBJ_DIR)

fclean:		clean
			$(RM) $(NAME)

re:		fclean $(NAME)

.PHONY:	all clean fclean re