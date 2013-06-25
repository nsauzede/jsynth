TARGET=
TARGET+=lsrbs.exe
TARGET+=catiff.exe
TARGET+=mkx0x.exe

SDL_CONFIG=sdl-config
WHICH_SDL_CONFIG:=x$(shell $(SDL_CONFIG) --cflags)x
ifneq ($(WHICH_SDL_CONFIG),xx)
HAVE_SDL=1
endif

CFLAGS=-Wall -Werror
#CFLAGS+=-O2
CFLAGS+=-g -O0

SCFLAGS+=`sdl-config --cflags`
SLDFLAGS+=`sdl-config --libs`

UNAME=$(shell uname)
ifeq ($(UNAME),MINGW32_NT-5.1)
WIN32=1
LDFLAGS+=-mno-windows
CFLAGS+=-mno-windows
endif

ifdef WIN32
#JACK="/c/Program Files/Jack v1.9.6"
JACK="/c/Program Files/Jack"
JCFLAGS+=-I$(JACK)/includes
#JLDFLAGS+=-L$(JACK)/lib
#JLDFLAGS+=-ljack
JLDFLAGS+=$(JACK)/lib/libjack.lib
JACK_H="$(JACK)/includes/jack/jack.h"
HAVE_JACK_H:=x$(shell if test -e "$(JACK_H)"; then echo -n oui ; else echo -n non ; fi)x
ifeq ($(HAVE_JACK_H),xouix)
HAVE_JACK=1
endif
else
HAVE_JACK_H=`if test -e /usr/include/jack/jack.h ; then echo -n oui ; else echo -n non ; fi`
ifeq ($(HAVE_JACK_H),oui)
HAVE_JACK=1
endif

JLDFLAGS+=-ljack
endif


ifdef HAVE_SDL
ifdef HAVE_JACK
TARGET+=jsynth.exe
endif
endif

all:$(TARGET)

rbs.o:rbs.h
lsrbs.o:rbs.h
lsrbs: lsrbs.o rbs.o
	$(CC) -o $@ $^ $(LDFLAGS)

x0x.o:x0x.h
mkx0x.o:x0x.h
mkx0x.exe: mkx0x.o x0x.o
	$(CC) -o $@ $^ $(LDFLAGS)

jsynth.exe:CFLAGS+=$(SCFLAGS)
jsynth.exe:CFLAGS+=$(JCFLAGS)
jsynth.exe:LDFLAGS+=$(SLDFLAGS)
jsynth.exe:LDFLAGS+=$(JLDFLAGS)

jynth.o:x0x.h
jsynth.exe: jsynth.o x0x.o
	$(CC) -o $@ $^ $(LDFLAGS)

%.exe:	%.c
	$(CC) -o $@ $(CFLAGS) $^ $(LDFLAGS)

clean:
	$(RM) $(TARGET) *.o *.exe stdout.txt stderr.txt

clobber: clean
	$(RM) *~
