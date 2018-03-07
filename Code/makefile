# Comp 4060 - Coding Project Makefile
# Credit to Dr. R. Thulasiraman for providing the original makefile template in COMP 4510

# name of the executable to be generated
PROG = get_tree_width
# space-delimited list of header files
HDRS = ProjectUtil.h TreeWidth.h Main.h
# space-delimited list of source files
SRCS = ProjectUtil.c TreeWidth.c Main.c

# directory to store object files
OBJDIR = object
# names of object files
OBJS = $(patsubst %.c, $(OBJDIR)/%.o, $(SRCS)) 

# name of the compiler
CC = gcc
# additional compiler flags to pass in
CFLAGS = -Wall --std=c99 -L.
# libraries for the linker
LIBS = -lm

####################
# Compiling rules: #
####################
# WARNING: *must* have a tab before each definition

# invoked when "make" is run
all : $(OBJDIR) $(PROG)

# links object files into executable
$(PROG) : $(OBJS)
	$(CC) $(CFLAGS) $^ -o $(PROG) $(LIBS)

# compiles source files into object files
object/%.o : %.c $(HDRS)
	$(CC) -c $(CFLAGS) $< -o $@ $(LIBS)

# creates directory to store object files
$(OBJDIR) :
	mkdir -p $@/

# cleans up object files and executable
# type "make clean" to use
# Note: you can add your own commands to remove other things (e.g. output files)
clean:
	rm -rf object/
	rm -f $(PROG)
