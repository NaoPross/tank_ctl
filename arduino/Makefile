## Settings for Arturo

# CC=ano
# 
# MODEL=nano
# CPU=atmega328
# 
# FLAGS=-m $(MODEL) --cpu $(CPU)

## Settings for ino
CC=ino
MODEL=nano328
FLAGS=-m $(MODEL)

all: build upload

build:
	$(CC) build $(FLAGS)

upload:
	$(CC) upload $(FLAGS)


