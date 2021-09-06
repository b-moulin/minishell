NAME = minishell

SRC =	bapt_utils.c \
		bapt_libft.c \
		commands/exit/exit.c \
		commands/cd/cd.c \
		commands/echo/echo.c \
		commands/pwd/pwd.c \
		commands/env/env.c \
		commands/export/export.c \
		commands/export/export_all.c \
		commands/export/export_error.c \
		commands/export/export_utils.c \
		commands/unset/unset.c \
		commands/execve/execve.c \
		redirections/redirections.c \
		redirections/init_redirections.c \
		signal/ctrlc.c \
		execution/execution.c \
		init/init.c \
		get_next_line.c \
		get_next_line.c \
		list.c \
		minishell.c \
		utils.c dollar.c \
		tokenisation.c \
		state_machine.c \
		retoken_env_var.c \
		env_var_value.c \
		parse.c \
		redirections.c \
		free.c \
		check_function_name.c \
		libft.c \
		free_part/free_part.c \
		free_part/free_part_utils.c \
		libft/ft_itoa.c \
		libft/ft_atoi.c \
		libft/ft_split.c \
		libft/ft_strnstr.c \
		libft/ft_substr.c \
		str_env_rep.c

LFLAGS = -I/Users/$(USER)/.brew/opt/readline/include -g3 -fsanitize=address

MAIN = main.c

OBJ = ${SRC:.c=.o}

CC  = gcc

RM  = rm -f

CFLAGS = $(LFLAGS) -g3 -fsanitize=address -Wall -Wextra -Werror  #--leak-check=full#	-fsanitize=address

all:		$(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
minishell.o: minishell.c
	$(CC) $(CFLAGS) -c $< -o $@ 

$(NAME): $(OBJ)
	$(CC) ${LFLAGS} $^ -o $(NAME) -L /Users/$(USER)/.brew/opt/readline/lib -lreadline


clean:
	$(RM) $(OBJ)

fclean:	clean
	$(RM) $(NAME)

re:		fclean all