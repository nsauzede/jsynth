TARGET=jsynth

CFLAGS=-Wall -Werror
CFLAGS+=-O2

CFLAGS+=`sdl-config --cflags`
LDFLAGS+=`sdl-config --libs`
LDFLAGS+=-ljack

all:$(TARGET)

%:	%.c
	$(CC) -o $@ $(CFLAGS) $^ $(LDFLAGS)

clean:
	$(RM) $(TARGET) *.exe stdout.txt stderr.txt

