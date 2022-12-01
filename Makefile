CC = gcc
BIN = ./bin/
LIB = ./
SRC = ./src/
CFLAGS = -Wall -Wextra -Werror -Wsign-conversion -Weffc++ -std=c++17
INCLUDE = -I$(SRC)
HEADERS = $(SRC)config.h $(SRC)cpprequest.h $(SRC)definition.h $(SRC)includes.h $(SRC)socket_io.h $(SRC)socket_util.h $(SRC)socket_wrapper.h $(SRC)utility.h
OBJS = $(BIN)cpprequest.o $(BIN)response.o $(BIN)socket_io.o $(BIN)socket_util.o $(BIN)socket_wrapper.o $(BIN)uri.o $(BIN)utility.o


libcpprequest.a: $(OBJS)
	mv *.o $(BIN)
	ar cr $(LIB)$@ $^


$(BIN)%.o: $(SRC)%.cpp $(HEADERS)
	$(CC) $(CFLAGS) $(INCLUDE) -c $<


.PHONY: clean
clean:
	rm -rf $(BIN)*
	rm $(LIB)libcpprequest.a
