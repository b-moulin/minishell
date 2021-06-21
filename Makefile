NAME = minishell

SRC =	main.c \
		utils.c \
		libft.c \
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
		commands/history/history.c

OBJ = ${SRC:.c=.o}

CC  = gcc

RM  = rm -f

CFLAGS = $(LDFLAGS) -lreadline -L/Users/$(USER)/.brew/opt/readline/lib -I/Users/$(USER)/.brew/opt/readline/include #-I/usr/local/opt/readline/include #-g3 -fsanitize=address #-Wall -Wextra -Werror -g3 #-fsanitize=address #-Wall -Wextra -Werror

all:		$(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	$(CC) ${CFLAGS} $^ -o $(NAME)

clean:
	$(RM) $(OBJ)

fclean:	clean
	$(RM) $(NAME)

re:		fclean all