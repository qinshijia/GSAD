gcc main.c Algorithm.c allAlg.c -w -g -o main.debug `pkg-config --cflags --libs gtk+-3.0`
gdb main.debug
rm main.debug
