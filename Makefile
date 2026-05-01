NAME = codexion
CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread
SRC = coders/main.c     \
	  coders/args.c     \
      coders/parsing.c  \
	  coders/init.c     \
	  coders/cleanup.c
OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re