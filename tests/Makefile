CC       = gcc
CFLAGS   = -Wall -Wextra -Werror -g -I../includes -lm
SRCS_DIR = ../srcs

LIB_SRCS = $(wildcard $(SRCS_DIR)/*.c)
TEST_SRCS = main.c \
            test_matrix.c \
            test_utils_primitives.c \
            test_strings.c \
            test_data_structures.c \
            test_graph_analysis.c \
            test_graph_conversion.c \
            test_graph_gen.c \
            test_sort.c \
            test_memory.c \
            test_io.c \
            test_permutation.c

LIB_OBJS  = $(LIB_SRCS:.c=.o)
TEST_OBJS = $(TEST_SRCS:.c=.o)
TARGET    = test_runner

all: $(TARGET)

$(TARGET): $(LIB_OBJS) $(TEST_OBJS)
	$(CC) $(CFLAGS) -o $@ $^
	@echo "Build OK: $(TARGET)"

$(SRCS_DIR)/%.o: $(SRCS_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TEST_OBJS) $(LIB_OBJS)

fclean: clean
	rm -f $(TARGET)

re: fclean all

.PHONY: all run clean fclean re
