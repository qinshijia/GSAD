SRC = $(wildcard *.c)
objs = $(patsubst %.c,%.o,$(SRC))
 
CC = gcc
CFLAGS = -Wall -g
LDFLAGS = 
 
TARGET = web_server
	 
all:$(TARGET) 
	 
$(TARGET):$(objs)
	$(CC) $(LDFLAGS) $^ -o $@ 
	    
%.o:%.c 
	$(CC) $(CFLAGS) -c $< -o $@ `pkg-config --cflags --libs gtk+-3.0` 
	        
.PHONY: clean
clean:
	-rm -f $(TARGET)
	-rm -f $(objs)
