CC = gcc
CFLAGS = -Wall -Wextra -Werror
INCLUDE = -I.
LIB = ../
OBJS = request.o socket_io.o socket_util.o uri.o utility.o

libcpprequest.a: $(OBJS)
	ar cr $(LIB)$@ $^

%.o: %.cpp %.h error.h
	$(CC) $(CFLAGS) $(INCLUDE) -c $<

.PHONY: clean
clean:
	rm -rf *.o *.gch
	rm $(LIB)libcpprequest.a
