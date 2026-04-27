CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -g -Iincludes -I../../includes -lm
ROOT    = ../..
LIB     = $(ROOT)/dist/libgenome.a

SRCS    = $(wildcard *.c)
OBJS    = $(SRCS:.c=.o)
TARGET  = test_runner

all: $(LIB) $(TARGET)

$(LIB):
	$(MAKE) -C $(ROOT)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIB)
	@echo "Build OK: $(TARGET)"

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all run clean
