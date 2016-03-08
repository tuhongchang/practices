CC:= gcc
CFLAGS= -O2 -Wall -c -I$(HEADS_PATH)
AR:= ar
ARFLAG:= -crs
HEADS_PATH:= ./
AR_OBJS:= libnetdevice.a libsqlite3.a

.PHONY:clean

all:$(AR_OBJS)

libnetdevice.a :netdevice.o
	$(AR) $(ARFLAG) $@ $^
netdevice.o: netdevice.c netdevice.h
	$(CC) $< -o $@ $(CFLAGS)

libsqlite3.a :sqlite3.o
	$(AR) $(ARFLAG) $@ $^
sqlite3.o: sqlite3.c sqlite3.h
	$(CC) $< -o $@ $(CFLAGS)
clean:
	-rm *.o *.a 
