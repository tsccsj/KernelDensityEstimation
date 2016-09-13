CC             ?= g++

TARGETS := io
SRCS    := $(TARGETS:=.c)
HDRS    := $(TARGETS:=.h)

all: KDE
KDE: KDE.c $(SRCS) $(HDRS)
	g++ -o ../$@ $+
clean:
	rm -f ../KDE
