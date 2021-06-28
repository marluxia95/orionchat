ODIR = ./src

_OBJ += chadchat.c room.c
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJ))

CC = gcc

COMPILER_FLAGS = -w 
LINKER_FLAGS = -lpthread

OBJ_NAME = server

all : $(OBJS)
	$(CC) -I./src $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

