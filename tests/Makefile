CC       = gcc
CFLAGS   = -Wall -Wextra -Werror -g -I../includes -lm
SRCS_DIR = ../srcs
OBJS_DIR = objs

LIB_SRCS  = $(wildcard $(SRCS_DIR)/*/*.c)
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

LIB_OBJS  = $(patsubst $(SRCS_DIR)/%.c, $(OBJS_DIR)/lib/%.o, $(LIB_SRCS))
TEST_OBJS = $(patsubst %.c, $(OBJS_DIR)/%.o, $(TEST_SRCS))
TARGET    = test_runner

all: $(TARGET)

$(TARGET): $(LIB_OBJS) $(TEST_OBJS)
	$(CC) $(CFLAGS) -o $@ $^
	@echo "Build OK: $(TARGET)"

$(OBJS_DIR)/lib/%.o: $(SRCS_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -f $(TARGET)

re: fclean all

.PHONY: all run clean fclean re
