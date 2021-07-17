
OBJS += orion.c room.c

L_OBJS += orion_array.c orion_core.c orion_prot.c 

L_C_OBJS += orion_array.a orion_core.a orion_prot.a 

CC = gcc

COMPILER_FLAGS = -w 
LINKER_FLAGS = -lpthread

OBJ_NAME = server

all : $(OBJS) $(L_OBJS)
	$(CC) -I. $(OBJS) $(L_OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

library : $(L_OBJS)
	$(CC) -I. -c $(L_OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) 
	ar -rc liborion.a *.o 
	rm -rf ./*.o