GUILE_FLAGS=`pkg-config --cflags --libs guile-3.0`
EXTENSIONS_DIR=`pkg-config guile-3.0 --variable=extensiondir`
SITE_DIR=`guile -c '(display (%site-dir))'`

CFLAGS=-O3
SO_FILE=libguileavl.so
NAME=avl

.PHONY: all clean install

all: $(SO_FILE)

$(SO_FILE): guile-avl.c common.c init.c pavl.c list_to_avl.c helper.c
	$(CC) $(CFLAGS) -g -fPIC -shared -o $@ $^ `pkg-config --cflags --libs guile-3.0`

install: $(SO_FILE) $(NAME).scm
	cp $^ $(EXTENSIONS_DIR)
	mkdir -p $(SITE_DIR)/extra
	cp $(NAME).scm $(SITE_DIR)/extra

uninstall:
	rm -f $(EXTENSIONS_DIR)/$(SO_FILE)
	rm -f $(SITE_DIR)/extra/$(NAME).scm

clean:
	rm -f *.o *.so

