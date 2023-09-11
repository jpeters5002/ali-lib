PROJECT = libali

CC = gcc

CFLAGS = -g -Wall -Wextra

# dirs
OUTPUT = build
OBJ = obj
SRC = src
INC = include

SOURCES := $(wildcard $(SRC)/*.c)
HEADERS := $(wildcard $(INC)/*.h)
