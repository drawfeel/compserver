######### Makefile COMMON ########
#c or c++
EXTENSION = cpp
#gcc or g++
CC = g++
#exe name
EXE = nerccmd
#source-file-direction space add-more-as: src src1 src2
SUBDIR = ./
#.o file direction
OBJ_DIR = out

#（including .h file direction）
#CPPFLAGS += -I ./inc
CPPFLAGS += -I ./

# link flags
LDFLAGS += -pthread

# C flags
CFLAGS += -g -Wall -O2 \
          -fsanitize=address
#not change lines below
 
CXX_SOURCES = $(foreach dir,$(SUBDIR), $(wildcard $(dir)/*.$(EXTENSION)))
CXX_OBJECTS = $(patsubst  %.$(EXTENSION), $(OBJ_DIR)/%.o, $(notdir $(CXX_SOURCES)))

$(EXE): $(CXX_OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(CXX_OBJECTS) -o $(EXE)
	mv $(EXE) ../
 
$(OBJ_DIR)/%.o: $(SUBDIR)/%.$(EXTENSION)
	if [ ! -d $(OBJ_DIR) ]; then mkdir -p $(OBJ_DIR); fi;\
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@


.PHONY:clean
clean:
	rm -rf $(OBJ_DIR) ../$(EXE)
