CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c++17
INCLUDE = -I.
HEADERS = cpprequest.h definition.h includes.h socket_io.h socket_util.h socket_wrapper.h utility.h
LIB = ../
OBJS = cpprequest.o response.o socket_io.o socket_util.o socket_wrapper.o uri.o utility.o

libcpprequest.a: $(OBJS)
	ar cr $(LIB)$@ $^

%.o: %.cpp $(HEADERS)
	$(CC) -std=c++17 $(INCLUDE) -c $<

.PHONY: clean
clean:
	rm -rf *.o *.gch
	rm $(LIB)libcpprequest.a
