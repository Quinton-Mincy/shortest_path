CC = gcc
CFLAGS += -Wall -Werror -Wextra
CFLAGS_DEBUG = $(CFLAGS)
MAIN = mouse
SRCS = main.c matrix.c queue.c maze_config.c bfs.c display.c error.c
OBJS = $(SRCS:.c=.o) 

all: $(MAIN)

$(MAIN): $(OBJS)
		$(CC) $(CFLAGS) -o $@ $(LINKERFLAG) $^

clean:
		$(RM) -f $(OBJS)

fclean: clean
		$(RM) -f $(MAIN) $(DEBUG)

re: fclean all