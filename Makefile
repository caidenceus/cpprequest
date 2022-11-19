CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c++17
INCLUDE = -I.
HEADERS = uri.h error.h request.h socket_io.h socket_util.h utility.h
LIB = ../
OBJS = cpprequest.o socket_io.o socket_util.o uri.o utility.o

libcpprequest.a: $(OBJS)
	ar cr $(LIB)$@ $^

%.o: %.cpp $(HEADERS)
	$(CC) $(CFLAGS) $(INCLUDE) -c $<

.PHONY: clean
clean:
	rm -rf *.o *.gch
	rm $(LIB)libcpprequest.a
