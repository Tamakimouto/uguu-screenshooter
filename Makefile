# uguu-screenshoote
# Makefile



# Gnu Compiler short hand
C = gcc

# -Wall Turn on warnings
# -v Verbose
# -g Debug
OPT = -Wall -v -g

# define lib
LIB = -L./lib

# define c source files
SRC = cap.c uploader.c util.c main.c

# rename to object
OBJ = $(SRC:.c=.o)

# define exe
MAIN = uguu-screenshooter

# default make
all: $(MAIN)
	@echo Compile Success

# Outputs executable - Links Objects
$(MAIN): global.o uguulib.o $(OBJ)
	$(C) $(OPT) -o $(MAIN) $(OBJ) global.o uguulib.o $(LIB)

# Compile global helper to Object
global.o: global.h
	$(C) $(OPT) -c global.h -o global.o

# Compile lib to Object
uguulib.o: uguulib.h
	$(C) $(OPT) -c uguulib.h -o uguulib.o

# Compile C source to Object
.c.o:
	$(C) $(OPT) -c $< -o $@

# Remove Object files
clean:
	$(RM) *.o*~ $(MAIN)