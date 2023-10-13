.SILENT: all

CC=clang++
CFLAGS=-Wall -Wextra -Wconversion -Wno-deprecated-declarations -s
INC=-Iincludes
SRC=PowerPwn.cc
BIN=PowerPwn


all: PowerPwn.cc
	$(CC) $(CFLAGS) $(INC) PowerPwn.cc -o PowerPwn