#include <stdio.h>
#include <sys/socket.h> // AF_INET and SOCK_STREAM
#include <arpa/inet.h> // storage size of serv_addr
#include <unistd.h> // getpid, getppid, fork
#include <string.h> // bzero
#define PORT 8080

int main(int argc, char const *argv[]) {
  
  for (int i ;i<10 ;i++) {
  int sock = 0, valread;
  struct sockaddr_in serv_addr;
  char *hello = "Hello from client";
  char buffer[1024] = {0};
  
  if ((sock = socket (AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("\n Socket creation error \n");
    return -1; 
  }
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);
  
  if (inet_pton (AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
    printf("\nInvalid address/ Address not supported \n");
  return -1; 
  }
  
  if (connect (sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    printf("\nConnection Failed \n");
  return -1; 
  }
    
  sleep(1);
  send (sock , hello , strlen(hello) , 0 );
  write(1, "Message sent\n", 13);
  printf("Hello message sent\n");
  }
return 0;
} 
