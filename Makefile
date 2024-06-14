CC=gcc
CFLAGS=-o


wash: wash.c
	$(CC) $(CFLAGS) $@ $+

install:
	cp wash /usr/local/bin/
	chmod 755 /usr/local/bin/wash
	mkdir -p /usr/local//man/man1
	cp wash.1 /usr/local/man/man1/
	chmod 644 /usr/local/man/man1/wash.1

clean:
	rm -f wash

uninstall:
	rm -f wash
	rm -f /usr/local/bin/wash
	rm -f /usr/local/man/man1/wash.1

