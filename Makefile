CC=gcc
CFLAGS=-Wall -Wextra -std=c2x -g
SRCDIR=./
HEADERDIR=headers
OBJDIR=obj
BINDIR=bin

SOURCES=$(wildcard $(SRCDIR)/*.c)
OBJECTS=$(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
TARGET=$(BINDIR)/tinymqtt

.PHONY: all clean dirs

all: dirs $(TARGET)

dirs:
	@mkdir -p $(OBJDIR) $(BINDIR)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -I$(HEADERDIR) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(BINDIR)

install: $(TARGET)
	sudo cp $(TARGET) /usr/local/bin/

uninstall:
	sudo rm -f /usr/local/bin/tinymqtt