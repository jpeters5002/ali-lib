PROJECT = libali
DYNLIB = $(PROJECT).so

CC = gcc

CFLAGS = -g -Wall -Wextra -Iinclude

# dirs
OUTPUT = build
OBJ = obj
SRC = src
INC = include

# commands
RM = rm -f

# files
SOURCES := $(wildcard $(SRC)/*.c)
HEADERS := $(wildcard $(INC)/*.h)
OBJECTS := $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(wildcard $(SRC)/*.c))

# compile command
COMPILE.c = $(CC) $(CFLAGS) -c -o $@
# link command
LINK.so.o = $(CC) -shared $(OBJECTS) -o $@

# rules
$(OUTPUT)/$(DYNLIB): $(SRC) $(OBJ) $(OUTPUT) $(OBJECTS)
	$(LINK.so.o)

$(SRC):
	mkdir -p $(SRC)

$(OBJ):
	mkdir -p $(OBJ)

$(OUTPUT):
	mkdir -p $(OUTPUT)

$(OBJ)/%.o: $(SRC)/%.c
	$(COMPILE.c) $<

clean:
	$(RM) $(OBJECTS)
	$(RM) $(OUTPUT)/$(DYNLIB)
