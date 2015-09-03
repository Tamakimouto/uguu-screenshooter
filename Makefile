# uguu-screenshoote
# Makefile



# Gnu Compiler short hand
C = gcc

# -Wall Turn on warnings
# -v Verbose
# -g Debug
OPT = -g

# define lib
LIB = -L./lib

# GTK 3 compiler flags
gOPT = `pkg-config gtk+-3.0 --cflags`

# GTK 3 links
gLIB = `pkg-config gtk+-3.0 --libs`

# define c source files
SRC = ./lib/cap.c ./lib/uploader.c ./lib/util.c ./src/main.c
CAP = ./lib/cap.c ./lib/cap.h
UTL = ./lib/util.c ./lib/util.h
UPL = ./lib/uploader.c ./lib/uploader.h

# rename to object
OBJ = $(SRC:.c=.o)

# define exe
MAIN = uguu-screenshooter

# default make
all: $(MAIN)
	@echo Compile Success

# Outputs executable - Links Objects
$(MAIN): ./lib/global.o ./lib/uguulib.o $(OBJ)
	$(C) $(gOPT) $(OPT) -o $(MAIN) $(OBJ) ./lib/global.o ./lib/uguulib.o $(LIB) $(gLIB)

# Compile global helper to Object
./lib/global.o: ./lib/global.h
	$(C) $(gOPT) $(OPT) -c ./lib/global.h $(gLIB)

# Compile lib to Object
./lib/uguulib.o: ./lib/uguulib.h
	$(C) $(gOPT) $(OPT) -lX11 -c ./lib/uguulib.h $(gLIB)

# cap object
cap.o: $(CAP)
	gcc `pkg-config gtk+-3.0 --cflags` -g -c ./lib/cap.c ./lib/cap.h `pkg-config gtk+-3.0 --libs`
	mv cap.o ./lib/

# Uploader Object
uploader.o: $(UPL)
	$(C) $(gOPT) $(OPT) -c $(UPL) $(gLIB)
	mv uploader.o ./lib/

# Util Object
util.o: $(UTL)
	$(C) $(gOPT) $(OPT) -c $(UTL) $(gLIB)
	mv util.o ./lib/


# Main
main.o:
	$(C) $(gOPT) $(OPT) -c main.c $(gLIB)

# Remove Object files
clean:
	$(RM) *.o*~ $(MAIN)