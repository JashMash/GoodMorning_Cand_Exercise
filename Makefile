CODE_DIRS = src tests
HEADERS = headers
# OBJECT_DIR = temp
CC =g++
BINARY = bin

# Making sure headers are checked for changes
DEP_FLAGS = -MP -MD
C_FLAGS = -Wall -Wextra -g $(foreach D, $(HEADERS), -I$(D)) $(DEP_FLAGS)

CODE_FILES = $(foreach D, $(CODE_DIRS), $(wildcard $(D)/*.cpp))

OBJECTS = $(patsubst %.cpp, %.o, $(CODE_FILES))
DEP_FILES = $(patsubst %.d, %.o, $(CODE_FILES))


all: $(BINARY)

$(BINARY) : $(OBJECTS)
	$(CC) -o $@ $^

$.o : $.c 
	$(CC) $(C_FLAGS) -c -o $@ $<

clean: 
	make -C src clean
	make -C tests clean
	del bin.exe
	del tar.bz2

distribute: clean
	tar zcvf tar.bz2  *