CFLAGS=-O3

all: libguileavl.so

libguileavl.so: guile-avl.c guile-bst.c common.c init.c pavl.c pbst.c list_to_avl.c helper.c
	$(CC) $(CFLAGS) -g -fPIC -shared -o $@ $^ `pkg-config --cflags --libs guile-3.0`

clean:
	rm -f *.so *.o
