
OBJS += client.c

CC = gcc

COMPILER_FLAGS = -w 
LINKER_FLAGS = -lpthread -lorion

OBJ_NAME = client

all : $(OBJS)
	$(CC) -I../server -L. -o $(OBJ_NAME) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) 