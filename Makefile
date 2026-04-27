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
# Miranda (2021) λ-permutation algorithms
# ---------------------------------------------------------------
# make run_lp              -- ビルド
# make run-lp ARGS="3 3 1 2"  -- 実行

WORK     = work
FDNS_DIR = $(WORK)/foundations
FDNS_C   = $(FDNS_DIR)/perm_analysis.c $(FDNS_DIR)/reversal.c $(FDNS_DIR)/transposition.c
LP_INV_C = $(WORK)/lp_inversions/lp_inversions.c
LP_SCO_C = $(WORK)/lp_score/lp_score.c
LP_BP_C  = $(WORK)/lp_breakpoints/lp_breakpoints.c
LP_SRC   = $(WORK)/run_lp.c $(FDNS_C) $(LP_INV_C) $(LP_SCO_C) $(LP_BP_C)
LP_INC   = -Iincludes -I$(FDNS_DIR) -I$(WORK)/lp_inversions \
           -I$(WORK)/lp_score -I$(WORK)/lp_breakpoints

run_lp: $(LIB)
	$(CC) $(CFLAGS) $(LP_INC) -lm $(LP_SRC) -L$(DISTDIR) -l$(TARGET:lib%=%) -o run_lp
	@echo "Built: run_lp"

run-lp: run_lp
	./run_lp $(ARGS)

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
        benchmark run-benchmark \
        run_lp run-lp
