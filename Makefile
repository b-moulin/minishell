NAME = minishell

SRC =	bapt_utils.c \
		bapt_libft.c \
		commands/exit/exit.c \
		commands/cd/cd.c \
		commands/echo/echo.c \
		commands/pwd/pwd.c \
		commands/env/env.c \
		commands/export/export.c \
		commands/unset/unset.c \
		commands/execve/execve.c \
		init/init.c \
		ft_split.c \
		ft_substr.c \
		get_next_line.c \
		commands/history/history.c \
		get_next_line.c \
		list.c \
		minishell.c \
		utils.c dollar.c \
		tokenisation.c \
		env_var_value.c \
		parse.c \
		redirections.c \
		free.c \
		check_function_name.c \
		libft.c \
		# redirections/read_redirections.c

LFLAGS = -I/Users/$(USER)/.brew/opt/readline/include -g3 -fsanitize=address

MAIN = main.c

OBJ = ${SRC:.c=.o}

CC  = gcc

RM  = rm -f

CFLAGS = -Wall -Wextra -Werror

# -I/Users/$(USER)/.brew/opt/readline/include -lreadline 
#MFLAGS = -I/Users/$(USER)/.brew/opt/readline/include -L/Users/$(USER)/.brew/opt/readline/lib  -lreadline #-I/usr/local/opt/readline/include #-g3 -fsanitize=address #-Wall -Wextra -Werror -g3 #-fsanitize=address #-Wall -Wextra -Werror gcc -L/Users/bmoulin/.brew/opt/readline/lib -I/Users/bmoulin/.brew/opt/readline/include  -c main.c -o main.o

all:		$(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
minishell.o: minishell.c
	$(CC) $(LFLAGS) -c $< -o $@ 

$(NAME): $(OBJ)
	$(CC) ${LFLAGS} $^ -o $(NAME) -L /Users/$(USER)/.brew/opt/readline/lib -lreadline


clean:
	$(RM) $(OBJ)

fclean:	clean
	$(RM) $(NAME)

re:		fclean all