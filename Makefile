.SUFFIXES:
.SUFFIXES: .cpp .o

# TODO: Consider using vpaths
CC = g++
LIB = ./
SRC = ./src/
OBJ = ./bin/
CPPFLAGS = -I./include/ -I./src/ -Wall -Wextra -Werror -Wsign-conversion -Weffc++ -std=c++17 -Wno-type-limits
OBJECTS = $(addprefix $(OBJ), cpprequest.o error.o response.o socket_wrapper.o uri.o utility.o)


$(OBJ)%.o: $(SRC)%.cpp ./include/cpprequest.h $(SRC)config.h $(SRC)error.h $(SRC)response.h $(SRC)socket_wrapper.h $(SRC)uri.h $(SRC)utility.hpp
	$(CC) $(CPPFLAGS) -o $@ $<


$(LIB)libcpprequest.a: $(OBJECTS)
	ar cr $@ $^


.PHONY: clean
clean:
	rm -rf $(OBJDIR)*
	rm $(LIB)libcpprequest.a
	touch $(OBJDIR)PLACEHOLDER.txt
