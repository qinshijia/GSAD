gcc main.c allAlg.c Algorithm.c -w  -o main `pkg-config --cflags --libs gtk+-3.0`
./main
rm main
