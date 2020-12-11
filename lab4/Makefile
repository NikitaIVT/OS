.PHONY: clean

all: serv client

serv: src/server.c
	gcc src/server.c -o serv
	
client: src/server.c
	gcc src/client.c -o client

clean:
	rm serv client
