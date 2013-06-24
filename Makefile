TARGET=
TARGET+=lsrbs.exe
TARGET+=catiff.exe
TARGET+=mkx0x.exe

#WHICH_SDL_CONFIG:=$(shell which sdl-config2)
#WHICH_SDL_CONFIG_=`which sdl-config3 2>&1 > /dev/null`
#WHICH_SDL_CONFIG__=`echo $(WHICH_SDL_CONFIG_) | cut -f 1 -d ":"`
#WHICH_SDL_CONFIG=`which sdl-config3 2>&1 | cut -f 1 -d ":"`
#WHICH_SDL_CONFIG:=x$(shell which sdl-config 2&1 | cut -f 1 -d ":")x
HAVE_SDL=1
ifdef HAVE_SDL
SDL_CONFIG=sdl-config
else
SDL_CONFIG=sdl-config2
endif
WHICH_SDL_CONFIG:=x$(shell $(SDL_CONFIG) --cflags)x
#WHICH_SDL_CONFIG:=x$(shell which sdl-config 2&1 > /dev/null)x
ifneq ($(WHICH_SDL_CONFIG),xx)
TARGET+=jsynth.exe
endif

#A=$(shell echo -n $(WHICH_SDL_CONFIG) > foo.txt)
#ifeq ($(A),toto)
#endif

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
