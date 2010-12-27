TARGET=jsynth

CFLAGS=-Wall -Werror

CFLAGS+=`sdl-config --cflags`
LDFLAGS+=`sdl-config --libs`
LDFLAGS+=-ljack

all:$(TARGET)

%:	%.c
	$(CC) -o $@ $(CFLAGS) $^ $(LDFLAGS)

clean:
	$(RM) $(TARGET) *.exe stdout.txt stderr.txt

