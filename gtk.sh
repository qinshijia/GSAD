gcc gtk.c allAlg.c mytu.c -w  -o gtk `pkg-config --cflags --libs gtk+-3.0`
./gtk
rm gtk
