CC:= gcc
CFLAGS= -O2 -Wall -c -I$(HEADS_PATH)
CLT_OBJ?=client
HEADS_PATH:= ../comm

$(CLT_OBJ):client.o usrfun.o
	$(CC) $^ -o $@

client.o usrfun.o: client.h $(HEADS_PATH)/dictionary_comm.h

client.o:client.c
	$(CC) $< -o $@ $(CFLAGS)
usrfun.o:usrfun.c
	$(CC) $< -o $@ $(CFLAGS)

.PHONY:clean

clean:
	-rm *.o client
