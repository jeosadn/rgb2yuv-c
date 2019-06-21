PREFIX ?= /usr/local
BINDIR ?= bin
INSTALL ?= install
CFLAGS += 
LDLAGS += 

TARGET = rgb2yuv-c

all: $(TARGET)

%: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

clean:
	$(RM) $(TARGET)

install: $(TARGET)
	mkdir -p $(DESTDIR)$(PREFIX)/$(BINDIR)
	$(INSTALL) $(TARGET) $(DESTDIR)$(PREFIX)/$(BINDIR)/$(TARGET)