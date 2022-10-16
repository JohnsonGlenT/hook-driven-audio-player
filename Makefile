##
# Hook Driven Audio Player
#
# @file
# @version 0.1

CC = gcc
LIBS= -lavcodec -lavformat -lavdevice -lavutil -lswresample

all: src/main.c
	$(CC) -Wall -g src/main.c -o audio-player $(LIBS)


# end
