CC:= gcc
CFLAGS:= -Wall -O2
LDFLAGS:= -lpthread -ldl

.PHONY: clean all

all: loaddatabase server client

server: server.o sqlite3.o
	$(CC) -o $@ $^ $(LDFLAGS)
	
client: client.o
	$(CC) -o $@ $^
	
loaddatabase: loaddatabase.o sqlite3.o
	$(CC) -o $@ $^ $(LDFLAGS)

%*.o: %*.c
	$(CC) -o $@ $^ $(CFLAGS)


clean:
	-rm server client loaddatabase *.o
