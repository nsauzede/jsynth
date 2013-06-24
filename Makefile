TARGET=jsynth.exe
TARGET+=lsrbs.exe
TARGET+=catiff.exe

CFLAGS=-Wall -Werror
#CFLAGS+=-O2
CFLAGS+=-g -O0

SCFLAGS+=`sdl-config --cflags`
SLDFLAGS+=`sdl-config --libs`

UNAME=$(shell uname)
ifeq ($(UNAME),MINGW32_NT-5.1)
WIN32=1
LDFLAGS+=-mno-windows
endif

ifdef WIN32
#JACK="/c/Program Files/Jack v1.9.6"
JACK="/c/Program Files/Jack"
JCFLAGS+=-I$(JACK)/includes
#JLDFLAGS+=-L$(JACK)/lib
#JLDFLAGS+=-ljack
JLDFLAGS+=$(JACK)/lib/libjack.lib
else
JLDFLAGS+=-ljack
endif

all:$(TARGET)

rbs.o:rbs.h
lsrbs.o:rbs.h
lsrbs: lsrbs.o rbs.o
	$(CC) -o $@ $^ $(LDFLAGS)

jsynth.exe:CFLAGS+=$(SCFLAGS)
jsynth.exe:CFLAGS+=$(JCFLAGS)
jsynth.exe:LDFLAGS+=$(SLDFLAGS)
jsynth.exe:LDFLAGS+=$(JLDFLAGS)

%.exe:	%.c
	$(CC) -o $@ $(CFLAGS) $^ $(LDFLAGS)

clean:
	$(RM) $(TARGET) *.o *.exe stdout.txt stderr.txt

clobber: clean
	$(RM) *~
