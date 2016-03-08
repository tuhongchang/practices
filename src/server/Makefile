CC= gcc
CFLAGS= -c -O2 -Wall $(HEADS_PATH)
LDFLAGS= -lsqlite3 -lnetdevice -lpthread -ldl  $(LIBS_PATH)
LIBS_PATH= -L$(COMM_PATH)
HEADS_PATH= -I$(COMM_PATH)
COMM_PATH?=../comm
SER_OBJ?=server

OBJS:= server.o useropt.o dbopt.o

$(SER_OBJ):$(OBJS)
	$(CC) $^ -o $@ $(LDFLAGS) 

$(OBJS):server.h $(COMM_PATH)/dictionary_comm.h

server.o:server.c 
	$(CC) $< -o $@ $(CFLAGS)
useropt.o:useropt.c 
	$(CC) $< -o $@ $(CFLAGS)
dbopt.o:dbopt.c 
	$(CC) $< -o $@ $(CFLAGS)

.PHONY: clean

clean:
	-killall -9 server
	-rm $(SER_OBJ) $(OBJS)
