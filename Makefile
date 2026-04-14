CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -Iincludes
AR      = ar
ARFLAGS = rcs

TARGET   = libgenome
SRCS_DIR = srcs
DISTDIR  = dist

SRCS = $(wildcard $(SRCS_DIR)/*.c)
OBJS = $(SRCS:.c=.o)
LIB  = $(DISTDIR)/$(TARGET).a

all: $(LIB)

$(LIB): $(OBJS)
	@mkdir -p $(DISTDIR)
	$(AR) $(ARFLAGS) $@ $^
	@echo "Archive created: $@"

$(SRCS_DIR)/%.o: $(SRCS_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(LIB)

re: fclean all

format:	find . -name "*.c" | xargs c_formatter_42

# Usage: make compile SRC=main.c
# Optional: make compile SRC=main.c OUT=my_program
compile: $(LIB)
	$(CC) $(CFLAGS) $(SRC) -L$(DISTDIR) -l$(TARGET:lib%=%) -lm -o $(if $(OUT),$(OUT),$(basename $(SRC)))

# Build and run main.c + approx_sbpbi.c
demo: $(LIB)
	$(CC) $(CFLAGS) main.c approx_sbpbi.c -L$(DISTDIR) -l$(TARGET:lib%=%) -lm -o demo
	./demo

.PHONY: all clean fclean re compile demo
