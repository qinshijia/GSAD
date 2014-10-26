SRC = $(wildcard *.c)
objs = $(patsubst %.c,%.o,$(SRC))
 
CC = gcc
CFLAGS = -Wall -g
LDFLAGS = 
 
TARGET = Graph-Algorithm-demo
	 
all:$(TARGET) 
	 
$(TARGET):$(objs)
	$(CC) $(LDFLAGS) $^ -o $@ `pkg-config --cflags --libs gtk+-3.0` 
	    
%.o:%.c 
	$(CC) $(CFLAGS) -c $< -o $@ `pkg-config --cflags --libs gtk+-3.0` 
	        
.PHONY: clean
clean:
	-rm -f $(TARGET)
	-rm -f $(objs)
