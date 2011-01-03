UNAME=$(shell uname)
ifeq ($(UNAME),MINGW32_NT-5.1)
WIN32=1
endif

TARGET=jsynth

CFLAGS=-Wall -Werror
CFLAGS+=-O2

CFLAGS+=`sdl-config --cflags`
LDFLAGS+=`sdl-config --libs`
LDFLAGS+=-ljack

ifdef WIN32
JACK="/c/Program Files/Jack v1.9.6"
CFLAGS+=-I$(JACK)/includes
LDFLAGS+=-L$(JACK)/lib
endif

all:$(TARGET)

%:	%.c
	$(CC) -o $@ $(CFLAGS) $^ $(LDFLAGS)

clean:
	$(RM) $(TARGET) *.exe stdout.txt stderr.txt

