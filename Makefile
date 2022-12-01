CC = gcc
BIN = ./bin/
LIB = ./
SRC = ./src/
CFLAGS = -Wall -Wextra -Werror -std=c++17
INCLUDE = -I$(SRC)
HEADERS = config.h cpprequest.h definition.h includes.h socket_io.h socket_util.h socket_wrapper.h utility.h

libcpprequest.a: $(BIN)*.o
	ar cr $(LIB)$@ $^

$(BIN)%.o: $(SRC)%.cpp $(HEADERS)
	$(CC) -std=c++17 $(INCLUDE) -c $<

.PHONY: clean
clean:
	rm -rf $(BIN)*
	rm $(LIB)libcpprequest.a
