
all :
	gcc -pthread server.c -o server -lws2_32
	gcc -pthread Client.c -o client -lws2_32
clean :
	del server.exe client.exe
