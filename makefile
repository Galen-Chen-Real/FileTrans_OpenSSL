CC =gcc
#CFLAGS =-lssl

all:server client
	
server:server.o tf_socket.o tf_trans.o
	$(CC) $+ -o $@
client:client.o tf_socket.o tf_trans.o
	$(CC) $+ -o $@
.c.o:
	$(CC) -c $< -o $*.o
clean:
	rm -f *.o server client

