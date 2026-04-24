NAME = codexion
CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread
SRC = coders/main.c     \
      coders/parsing.c  \
      coders/init.c     \
	  coders/utils.c    \
#       coders/coder.c    \
#       coders/dongle.c   \
#       coders/scheduler.c \
#       coders/monitor.c  \
#       coders/logger.c   \
#       coders/utils.c
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