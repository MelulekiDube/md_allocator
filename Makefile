CXX		  := clang++
CXX_FLAGS := -Wall -Wextra -std=c++17 -g -Wunneeded-internal-declaration

BIN		:= bin
SRC		:= src
INCLUDE	:= include 
LIB		:= lib

LIBRARIES	:=
EXECUTABLE	:= main


all: $(BIN)/$(EXECUTABLE)

run: clean all
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -L$(LIB) $^ -g -o $@ $(LIBRARIES)

clean:
	rm -dfr $(BIN)/*