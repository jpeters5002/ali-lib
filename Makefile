PROJECT = libali
DYNLIB = $(PROJECT).so

CC = gcc

CFLAGS = -g -Wall -Wextra -Iinclude

# dirs
OUTPUT = build
OBJ = obj
SRC = src
INC = include
LIB_INSTALL = /usr/local/lib/ali
INC_INSTALL = /usr/local/include/ali

# commands
RM = rm -f
RM_REC = rm -rf
CP = cp

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
	$(RM) $(OUTPUT)/*

$(LIB_INSTALL):
	mkdir -p $@

$(INC_INSTALL):
	mkdir -p $@

install: $(LIB_INSTALL) $(INC_INSTALL) $(OUTPUT)/$(DYNLIB)
	$(CP) $(OUTPUT)/$(DYNLIB) $(LIB_INSTALL); \
	$(CP) $(HEADERS) $(INC_INSTALL)

uninstall:
	$(RM_REC) $(LIB_INSTALL) $(INC_INSTALL)
$(OUTPUT)/test_ali:
	$(CC) test/test_ali.c -L/usr/local/lib/ali -lali -I/usr/local/include/ali -o $(OUTPUT)/test_ali
test: $(OUTPUT)/test_ali
