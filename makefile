CFLAGS= -g -Wall -Werror

all: client

client: client.cpp
	$(CXX) $(CFLAGS) -o client.o -c client.cpp
	$(CXX) $(CFLAGS) -o client client.o

clean:
	rm -f *.o client
