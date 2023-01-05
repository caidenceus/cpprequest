.SUFFIXES:
.SUFFIXES: .cpp .o

# TODO: Consider using vpaths
CC = g++
LIB = ./
SRC = ./src/
OBJ = ./bin/
INCLUDE = -I./include/ -I$(SRC)
CPPFLAGS = -Wall -Wextra -Werror -Wsign-conversion -Weffc++ -std=c++17 -Wno-type-limits
OBJECTS = $(addprefix $(OBJ), cpprequest.o error.o response.o socket_wrapper.o uri.o utility.o)


$(LIB)libcpprequest.a: $(OBJECTS)
	ar cr $@ $^


$(OBJ)%.o: $(SRC)%.cpp ./include/cpprequest.h $(SRC)config.h $(SRC)error.h $(SRC)response.h $(SRC)socket_wrapper.h $(SRC)uri.h $(SRC)utility.hpp
	$(CC) -c $(INCLUDE) $(CPPFLAGS) $< -o $@


.PHONY: clean
clean:
	rm -rf $(OBJ)*
	rm $(LIB)libcpprequest.a
	touch $(OBJ)PLACEHOLDER.txt
