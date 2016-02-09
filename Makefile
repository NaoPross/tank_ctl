CC=gcc

LIB=-L/usr/X11R6/lib -lX11 -lm 
INCLUDE=-I/usr/X11R6/include -I./include
FLAGS=-Wall -g `pkg-config --cflags --libs cairo`

SOURCES=src/*.c
OUTPUT=bin/tank_ctl

all: $(OUTPUT)

$(OUTPUT): $(SOURCES)
	$(CC) -o $(OUTPUT) $(FLAGS) $(INCLUDE) $(LIB) $(SOURCES)

run:
	./$(OUTPUT)
