NAME = codexion
CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread
SRC = coders/main.c            \
	  coders/args.c            \
      coders/parsing.c         \
	  coders/init.c            \
	  coders/cleanup.c         \
	  coders/logger.c          \
	  coders/coder.c           \
	  coders/monitor.c         \
	  coders/dongles.c         \
	  coders/dongles_utils.c   \
	  coders/coder_utils.c     \
	  coders/scheduler.c       \
	  coders/scheduler_utils.c
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