##
# Hook Driven Audio Player
#
# @file
# @version 0.1

libs = -lavcodec-ffmpeg -lavformat-ffmpeg -lavutil -lswresample
CC = gcc

all: src/main.c
	$(CC) src/main.c -o audio-player $(LIBS)


# end
