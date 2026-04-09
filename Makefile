CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -Iincludes
AR      = ar
ARFLAGS = rcs

TARGET   = lib
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

.PHONY: all clean fclean re
