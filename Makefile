OBJS	= words.o
SOURCE	= words.c 
OUT	= words
CC	 = gcc
FLAGS	 = -g -c -Wall
LFLAGS	 = 

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

words.o: words.c
	$(CC) $(FLAGS) words.c -std=c99

clean:
	rm -f $(OBJS) $(OUT)