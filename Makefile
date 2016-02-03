CC=gcc

LIB=-L/usr/X11R6/lib -lX11 -lm
INCLUDE=-I/usr/X11R6/include -I./include

SOURCES=src/tank_ctl.c src/serialf.c
OUTPUT=bin/tank_ctl

all: $(OUTPUT)

$(OUTPUT): $(SOURCES)
	$(CC) -o $(OUTPUT) $(INCLUDE) $(LIB) $(SOURCES)
