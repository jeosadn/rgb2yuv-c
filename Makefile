PREFIX=/usr/local

src=$(wildcard *.c)
obj=$(src:.c=.o)

rgb2yuv-c: $(obj)
	$(CC) -o $@ $^

.PHONY: clean
clean:
	rm -f $(obj) rgb2yuv-c

.PHONY: install
install: rgb2yuv-c
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp $< $(DESTDIR)$(PREFIX)/bin/rgb2yuv-c

.PHONY: uninstall
uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/rgb2yuv-c
