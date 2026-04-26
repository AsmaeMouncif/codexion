NAME = codexion
CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread
SRC = coders/main.c     \
      coders/parsing.c  \
      coders/init.c     \
      coders/coder.c    \
      coders/dongle.c   \
      coders/scheduler.c \
      coders/monitor.c  \
      coders/logger.c   \
      coders/utils.c
OBJ = $(SRC:.c=.o)

# Test executables
TEST_UNIT_NAME = test_unit
TEST_INTEGRATION_NAME = test_integration
TEST_SRC = coders/parsing.c  \
           coders/init.c     \
           coders/dongle.c   \
           coders/scheduler.c \
           coders/logger.c   \
           coders/utils.c

TEST_UNIT_OBJ = $(TEST_SRC:.c=.o) coders/test.o
TEST_INTEGRATION_OBJ = $(TEST_SRC:.c=.o) coders/test_integration.o

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Build unit tests executable
build_unit_tests: $(TEST_UNIT_OBJ)
	$(CC) $(CFLAGS) -o $(TEST_UNIT_NAME) $^

# Build integration tests executable
build_integration_tests: $(TEST_INTEGRATION_OBJ)
	$(CC) $(CFLAGS) -o $(TEST_INTEGRATION_NAME) $^

# Run all tests
test: build_unit_tests build_integration_tests
	@echo ""
	@echo "════════════════════════════════════════"
	@echo "Running Unit Tests..."
	@echo "════════════════════════════════════════"
	@./$(TEST_UNIT_NAME)
	@echo ""
	@echo "════════════════════════════════════════"
	@echo "Running Integration Tests..."
	@echo "════════════════════════════════════════"
	@./$(TEST_INTEGRATION_NAME)
	@echo ""

# Run unit tests only
test_unit: build_unit_tests
	@echo ""
	@echo "════════════════════════════════════════"
	@echo "Running Unit Tests..."
	@echo "════════════════════════════════════════"
	@./$(TEST_UNIT_NAME)
	@echo ""

# Run integration tests only
test_integration: build_integration_tests
	@echo ""
	@echo "════════════════════════════════════════"
	@echo "Running Integration Tests..."
	@echo "════════════════════════════════════════"
	@./$(TEST_INTEGRATION_NAME)
	@echo ""

# Run tests with valgrind for memory checking
test_valgrind: build_unit_tests build_integration_tests
	@echo ""
	@echo "════════════════════════════════════════"
	@echo "Running with Valgrind (Memory Check)..."
	@echo "════════════════════════════════════════"
	@valgrind --leak-check=full --show-leak-kinds=all ./$(TEST_UNIT_NAME)
	@valgrind --leak-check=full --show-leak-kinds=all ./$(TEST_INTEGRATION_NAME)

clean:
	rm -f $(OBJ) coders/test.o coders/test_integration.o

fclean: clean
	rm -f $(NAME) $(TEST_UNIT_NAME) $(TEST_INTEGRATION_NAME)

re: fclean all

# Test targets don't need rebuilding
.PHONY: all clean fclean re test test_unit test_integration build_unit_tests build_integration_tests test_valgrind