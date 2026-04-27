CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -Iincludes
AR      = ar
ARFLAGS = rcs

TARGET   = libgenome
SRCS_DIR = srcs
OBJS_DIR = objs
DISTDIR  = dist

SRCS = $(wildcard $(SRCS_DIR)/*/*.c)
OBJS = $(patsubst $(SRCS_DIR)/%.c, $(OBJS_DIR)/%.o, $(SRCS))
LIB  = $(DISTDIR)/$(TARGET).a

all: $(LIB)

$(LIB): $(OBJS)
	@mkdir -p $(DISTDIR)
	$(AR) $(ARFLAGS) $@ $^
	@echo "Archive created: $@"

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -f $(LIB)
	rm -f check all_check make_input benchmark
	rm -f log input

re: fclean all

format:
	find . -name "*.c" | xargs c_formatter_42
	find . -name "*.h" | xargs c_formatter_42

# ---------------------------------------------------------------
# sbpbi (Sorting by Prefix Block Interchanges)
# ---------------------------------------------------------------
# make sbpbi           -- 全ツールをビルド
# 将来 make <algo> の形で他アルゴリズムを追加していく

SBPBI = sbpbi

check: $(LIB)
	$(CC) $(CFLAGS) $(SBPBI)/check.c $(SBPBI)/approx_sbpbi.c $(SBPBI)/solve.c -L$(DISTDIR) -l$(TARGET:lib%=%) -lm -o check

run-check: check
	./check $(ARGS)

all_check: $(LIB)
	$(CC) $(CFLAGS) $(SBPBI)/all_check.c $(SBPBI)/approx_sbpbi.c $(SBPBI)/solve.c -L$(DISTDIR) -l$(TARGET:lib%=%) -lm -o all_check

run-all_check: all_check
	./all_check $(N)

make_input: $(LIB)
	$(CC) $(CFLAGS) $(SBPBI)/make_input.c -L$(DISTDIR) -l$(TARGET:lib%=%) -lm -o make_input

gen: make_input
	./make_input $(N)

check-input: check make_input
	cat input | xargs ./check

benchmark: $(LIB)
	$(CC) $(CFLAGS) $(SBPBI)/benchmark.c $(SBPBI)/approx_sbpbi.c $(SBPBI)/solve.c -L$(DISTDIR) -l$(TARGET:lib%=%) -lm -o benchmark

run-benchmark: benchmark
	./benchmark $(N) $(T)

sbpbi: check all_check make_input benchmark

# ---------------------------------------------------------------
# Misc
# ---------------------------------------------------------------

# Usage: make compile SRC=main.c
# Optional: make compile SRC=main.c OUT=my_program
compile: $(LIB)
	$(CC) $(CFLAGS) $(SRC) -L$(DISTDIR) -l$(TARGET:lib%=%) -lm -o $(if $(OUT),$(OUT),$(basename $(SRC)))

.PHONY: all clean fclean re compile format \
        sbpbi \
        check run-check all_check run-all_check \
        make_input gen check-input \
        benchmark run-benchmark
