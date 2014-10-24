gcc gtk.c mytu.c -w -g -o gtk.debug `pkg-config --cflags --libs gtk+-3.0`
gdb gtk.debug
rm gtk.debug
