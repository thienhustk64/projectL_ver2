
all :
	gcc message.h -Wall -c message.c
	gcc -pthread server.c message.o -o server -lws2_32
	gcc -pthread client.c message.o -o client -lws2_32
	
clean :
	del server.exe client.exe message.o message.gch
