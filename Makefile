TARGET=jsynth
TARGET+=lsrbs
TARGET+=catiff

CFLAGS=-Wall -Werror
#CFLAGS+=-O2
CFLAGS+=-g -O0

CFLAGS+=`sdl-config --cflags`
LDFLAGS+=`sdl-config --libs`

UNAME=$(shell uname)
ifeq ($(UNAME),MINGW32_NT-5.1)
WIN32=1
LDFLAGS+=-mno-windows
endif

ifdef WIN32
#JACK="/c/Program Files/Jack v1.9.6"
JACK="/c/Program Files/Jack"
CFLAGS+=-I$(JACK)/includes
#LDFLAGS+=-L$(JACK)/lib
#LDFLAGS+=-ljack
LDFLAGS+=$(JACK)/lib/libjack.lib
else
LDFLAGS+=-ljack
endif

all:$(TARGET)

rbs.o:rbs.h
lsrbs.o:rbs.h
lsrbs: lsrbs.o rbs.o
	$(CC) -o $@ $^ $(LDFLAGS)

%:	%.c
	$(CC) -o $@ $(CFLAGS) $^ $(LDFLAGS)

clean:
	$(RM) $(TARGET) *.o *.exe stdout.txt stderr.txt

clobber: clean
	$(RM) *~
