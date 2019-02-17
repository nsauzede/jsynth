_SYS:=$(shell uname -o)
ifeq ($(_SYS),Msys)
WIN32:=1
endif


TARGET:=
TARGET+=catrbs.exe
TARGET+=catiff.exe
TARGET+=mkx0x.exe
TARGET+=rbs2x0x.exe
TARGET+=catx0x.exe
TARGET+=catmid.exe

CFLAGS:=-Wall -Werror
#CFLAGS+=-O2
CFLAGS+=-g -O0

ifdef WIN32
#JACK:="/c/Program Files/Jack v1.9.6"
JACK:="/c/Program Files (x86)/Jack"
JCFLAGS+=-I$(JACK)/includes
#JLDFLAGS+=-L$(JACK)/lib
#JLDFLAGS+=-ljack
ifdef USE32
JLDFLAGS+=$(JACK)/lib/libjack.lib
else
JLDFLAGS+=$(JACK)/lib/libjack64.lib
endif
JACK_H:="$(JACK)/includes/jack/jack.h"
HAVE_JACK_H:=x$(shell if test -e "$(JACK_H)"; then echo -n oui ; else echo -n non ; fi)x
ifeq ($(HAVE_JACK_H),xouix)
HAVE_JACK:=1
endif
else
HAVE_JACK_H:=x$(shell if test -e "/usr/include/jack/jack.h"; then echo -n oui ; else echo -n non ; fi)x
ifeq ($(HAVE_JACK_H),xouix)
HAVE_JACK:=1
endif

JLDFLAGS+=-ljack
endif

include sdl.mak
ifdef SDLCONFIG
HAVE_SDL:=1
ifeq ($(SDL_VER),1)
SDL_FLAGS+=-DSDL1
else
SDL_FLAGS+=-DSDL2
endif

SCFLAGS+=$(SDL_FLAGS)
SLDFLAGS+=$(SDL_LIBS)
endif

ifdef HAVE_SDL
ifdef HAVE_JACK
TARGET+=jsynth.exe
endif
endif

all: SDL_CHECK $(TARGET)

rbs.o:rbs.h
catrbs.o:rbs.h
catrbs: catrbs.o rbs.o
	$(CC) -o $@ $^ $(LDFLAGS)

x0x.o:x0x.h
mkx0x.o:x0x.h
mkx0x.exe: mkx0x.o x0x.o
	$(CC) -o $@ $^ $(LDFLAGS)

catx0x.o:x0x.h
catx0x.exe: catx0x.o x0x.o
	$(CC) -o $@ $^ $(LDFLAGS)

rbs2x0x.o:x0x.h rbs.h
rbs2x0x.exe: rbs2x0x.o x0x.o
	$(CC) -o $@ $^ $(LDFLAGS)

jsynth.exe:CFLAGS+=$(SCFLAGS)
jsynth.exe:CFLAGS+=$(JCFLAGS)
jsynth.exe:LDFLAGS+=$(SLDFLAGS)
jsynth.exe:LDFLAGS+=$(JLDFLAGS)
jsynth.exe:LDFLAGS+=-lm

jynth.o:x0x.h
jsynth.exe: jsynth.o x0x.o
	$(CC) -o $@ $^ $(LDFLAGS)

ifdef WIN32
# this is for ntohl
catmid.exe: LDFLAGS+=-lws2_32
endif

%.exe:	%.c
	$(CC) -o $@ $(CFLAGS) $^ $(LDFLAGS)

clean:
	@$(RM) $(TARGET) *.o *.exe stdout.txt stderr.txt

clobber: clean
	@$(RM) *~
