#OBJS specifies which files to compile as part of the project

OBJS = $(wildcard ./src/*.cpp)  $(wildcard ./src/**/*.cpp)

#CC specifies which compiler we're using
CC = g++

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
# -Wl,-subsystem,windows gets rid of the console window
COMPILER_FLAGS = -Wl,-subsystem,console

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lmingw32

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = awesomeKeyExtractor

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

clean:
	del $(OBJ_NAME).exe