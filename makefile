CC = gcc
RM = rm -rf
DEBUG = -g -pg
CFLAGS = -Wall -Wextra $(DEBUG)
LIBS = -lm -lpthread -lssl
SRC1 = server.c
SRC2 = client.c
OBJ1 = $(SRC1:.c=.o)
OBJ2 = $(SRC2:.c=.o)
NAME1 = server.exe
NAME2 = client.exe
NAME_TAR = G4_F1-2_st20723_ls24389.tar

.PHONY: all clean

all: $(NAME1) $(NAME2)
	@echo ' '
	@echo 'Compiling target: $(NAME1)'
	@echo 'Finished Compiling target: $(NAME1)'
	@echo ' '
	@echo 'Compiling target: $(NAME2)'
	@echo 'Finished Compiling target: $(NAME2)'
	@echo ' '

$(NAME1): $(OBJ1)
	@echo 'Building target: $@'
	$(CC) $(CFLAGS) -o $(NAME1) $(OBJ1) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

$(NAME2): $(OBJ2)
	@echo 'Building target: $@'
	$(CC) $(CFLAGS) -o $(NAME2) $(OBJ2) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

.c.o:
	@echo ' '
	@echo 'Building target: $@'
	$(CC) $(CFLAGS) -c $< -o $@
	@echo 'Finished building target: $@'
	@echo ' '

clean:
	@echo ' '
	@echo 'Removing: $(NAME1) $(NAME2) $(OBJ2) $(OBJ1) $(NAME_TAR)'
	$(RM) *.o core *.out *~ $(NAME1) $(NAME2) $(NAME_TAR)
	@echo 'Finished removing: $(NAME1) $(NAME2) $(OBJ2) $(OBJ1) $(NAME_TAR)'
	@echo ' '

tar:
	@echo ' '
	@echo 'Building compressed tar file: $(NAME_TAR)'
	tar cfv $(NAME_TAR) *.c *.h makefile
	@echo ' '
