SRC = $(wildcard *.c)
objs = $(patsubst %.c,%.o,$(SRC))
 
CC = gcc
CFLAGS = -Wall -g
LIB =  `pkg-config --cflags --libs gtk+-3.0` 
 
TARGET = GSAD
	 
all:$(TARGET) clean run
	 
$(TARGET):$(objs)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIB)
	    
%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@  $(LIB)
	        
run:
	./$(TARGET)
clean:
	-rm -f $(objs)
