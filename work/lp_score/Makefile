CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -g -Iincludes -I../../includes -lm
ROOT    = ../..
LIB     = $(ROOT)/dist/libgenome.a

FDNS_DIR = ../foundations
FDNS_SRCS = $(FDNS_DIR)/perm_analysis.c $(FDNS_DIR)/reversal.c $(FDNS_DIR)/transposition.c
FDNS_OBJS = $(FDNS_SRCS:.c=.o)

SRCS    = $(wildcard *.c)
OBJS    = $(SRCS:.c=.o)
TARGET  = test_runner

all: $(LIB) $(TARGET)

$(LIB):
	$(MAKE) -C $(ROOT)

$(TARGET): $(OBJS) $(FDNS_OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(FDNS_OBJS) $(LIB)
	@echo "Build OK: $(TARGET)"

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all run clean
