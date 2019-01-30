NAME=Medusa

IDIR =../include
CC=g++
CFLAGS=-I$(IDIR)

ODIR=obj

#Look in current directory for internal libraries
LIBFLAG=$(wildcard *.dll)

#Add the external libraries
LIBFLAG += -L../lib
LIBFLAG += -lm -lglu32 -lgdi32 -lopengl32
#-lglfw3

_DEPS =$(wildcard *.h)
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

SRC=$(wildcard *.cpp)
_OBJ = $(SRC:.cpp=.o)
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -std=c++17 -fpic -o $@ $< $(CFLAGS)


#1-Compile and link
#2-Run the exe
$(NAME): $(OBJ)
	$(CC) -o $@.exe glad.c $^ $(CFLAGS) $(LIBFLAG)
	$(NAME).exe

#Clean and make
over:
	cls
	$(MAKE) clean
	$(MAKE)
	
#Creates a static library
statlib: $(OBJ)
	ar rcs lib$(NAME).a $(OBJ)

#Creates a dynamic library
#It will create a file name "ProjectName".dll (no lib in front), which is internal (only works with this compiler)
#Copy/paste "ProjectName".dll into "DestinationProject"/src
#Call with g++ -o (...).exe (...).o "ProjectName".dll ...
dynlib: $(OBJ)
	$(CC) -shared -o $(NAME).dll $^ $(CFLAGS) $(LIBFLAG)

#make NAME=... newproject
newproject:
	@echo off
	mkdir $(NAME)
	mkdir $(NAME)\\include
	mkdir $(NAME)\\lib
	mkdir $(NAME)\\src
	mkdir $(NAME)\\src\\obj
	echo NAME=$(NAME)>temp
	type makefile>>temp
	move temp $(NAME)\\src
	ren $(NAME)\\src\\temp makefile

#Create a new class (h & cpp files) make CLASS=... newfile
newclass:
	@echo off
	@echo #pragma once> $(CLASS).h
	@echo.>> $(CLASS).h
	@echo class $(CLASS)>> $(CLASS).h
	@echo {>> $(CLASS).h
	@echo 	public:>> $(CLASS).h
	@echo 		$(CLASS)();>> $(CLASS).h
	@echo.>> $(CLASS).h
	@echo 	private:>> $(CLASS).h
	@echo };>> $(CLASS).h
	move $(CLASS).h ..\\include
	@echo #include ^"$(CLASS).h^"> $(CLASS).cpp
	@echo.>> $(CLASS).cpp
	@echo $(CLASS)::$(CLASS)()>> $(CLASS).cpp	
	@echo {>> $(CLASS).cpp
	@echo }>> $(CLASS).cpp
	
#make DEST=... install
install:
	@echo off
	del $(NAME).dll
	$(MAKE) dynlib
	if not exist ..\\..\\$(DEST)\\include\\$(NAME) md ..\\..\\$(DEST)\\include\\$(NAME)
	copy ..\\include\\*.h ..\\..\\$(DEST)\\include\\$(NAME)
	copy $(NAME).dll ..\\..\\$(DEST)\\src

.PHONY: clean

#delete all the Object files and the executable.
clean:
	del $(ODIR)\\*.o *.exe