all: server client

server: eshop_server.o
  echo "Compiling and producing server......"
  gcc eshop_server.o -o server
  chmod +x server

eshop_server.o: eshop_server.c
  echo "Compiling the server......"
  gcc -c eshop_server.c

client: client.o
  echo "Compiling and creating client......"
  gcc client.o -o client
  chmod +x client

client.o: client.c
  echo "Compiling client......"
  gcc -c client.c

clean:
  echo "removing client and server....."
  rm server client eshop_server.o client.o
