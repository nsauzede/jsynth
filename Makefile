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
endif

ifdef WIN32
#JACK="/c/Program Files/Jack v1.9.6"
JACK="/c/Program Files/Jack"
JCFLAGS+=-I$(JACK)/includes
#JLDFLAGS+=-L$(JACK)/lib
#JLDFLAGS+=-ljack
JLDFLAGS+=$(JACK)/lib/libjack.lib
HAVE_JACK=`if test -e $(JACK)/includes/jack/jack.h ; then echo oui ; else echo non ; fi`
ifeq ($(HAVE_JACK),oui)
HAVE_JACK=1
endif
else
HAVE_JACK=`if test -e /usr/include/jack/jack.h ; then echo oui ; else echo non ; fi`
ifeq ($(HAVE_JACK),oui)
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
