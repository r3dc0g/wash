CC=gcc
CFLAGS=-o


wash: wash.c
	$(CC) $(CFLAGS) $@ $+

install:
	cp wash /usr/local/bin/
	chmod 755 /usr/local/bin/wash
	mkdir -p /usr/local//man/man1
	chmod 644 /usr/local/man/man1/dwm.1

clean:
	rm -f wash
	rm /usr/local/bin/wash
	rm /usr/local/man/man1/wash.1

