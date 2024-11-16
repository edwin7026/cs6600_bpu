CC = g++
OPT = -O3
#OPT = -g
WARN = -Wall
CFLAGS = $(OPT) $(WARN) $(INC) $(LIB)

# List corresponding compiled object files here (.o files)
SIM_OBJ = cpu.o common.o main.o
 
#################################

# default rule

all: bpu_sim
	@echo "my work is done here..."

# rule for making cache_sim

bpu_sim: $(SIM_OBJ)
	$(CC) -o bpsim $(CFLAGS) $(SIM_OBJ) -lm
	@echo "-----------DONE WITH CACHE_SIM-----------"


# generic rule for converting any .cc file to any .o file
%.o: ../src/%.cpp
	$(CC) $(CFLAGS) -I../src/ -c $^


# type "make clean" to remove all .o files plus the cache_sim binary

clean:
	rm -f *.o bpsim


# type "make clobber" to remove all .o files (leaves cache_sim binary)

clobber:
	rm -f *.o
