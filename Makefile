NAME = minishell

SRC =	main.c \
		utils.c \
		libft.c \
		commands/cd/cd.c \
		commands/echo/echo.c \
		commands/pwd/pwd.c \
		commands/env/env.c \
		commands/export/export.c \
		init/init.c

OBJ = ${SRC:.c=.o}

CC  = gcc

RM  = rm -f

CFLAGS = #-g3 -fsanitize=address #-Wall -Wextra -Werror -g3 #-fsanitize=address #-Wall -Wextra -Werror

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