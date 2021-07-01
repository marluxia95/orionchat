
OBJS += orion.c orion_array.c orion_core.c orion_prot.c room.c

CC = gcc

COMPILER_FLAGS = -w 
LINKER_FLAGS = -lpthread

OBJ_NAME = server

all : $(OBJS)
	$(CC) -I. $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

