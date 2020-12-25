LDFLAGS=-ldl

.PHONY: clean

all: serv client libhelp.so

#serv: builds/serv
	#gcc builds/serv -I/libhelp.so -o serv

builds/serv.o: src/server.c
	gcc -c src/server.c -ldl -o builds/serv.o 

client: src/client.c
	gcc src/client.c -o client

libhelp.so: dlib/help.o
	gcc ./dlib/help.o -shared -o ./dlib/libhelp.so
	
dlib/help.o: dlib/h.c
	gcc -c -fPIC ./dlib/h.c -o ./dlib/help.o
	
clean:
	rm serv client

serv: builds/serv
	gcc src/server.c -L./dlib/libhelp.so -o serv -ldl
