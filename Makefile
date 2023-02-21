
all :
	gcc packet.h -Wall -c packet.c
	gcc -pthread server.c packet.o -o server -lws2_32
	gcc -pthread Client.c packet.o -o client -lws2_32
	
clean :
	del server.exe client.exe packet.o packet.gch
