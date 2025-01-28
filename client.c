#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/socket.h> // AF_INET and SOCK_STREAM
#include <arpa/inet.h> // storage size of serv_addr
#include <unistd.h> // getpid, getppid, fork
#include <string.h> // bzero
#include <time.h>

//Constast variables
#define CLIENTS 5       
#define MAX_ITEMS 20
#define ORDERS 10
#define PORT 8080

//Global variables
int random_item;
int error_flag;
int random_item;

int Send_Order(int random_item){
  int sock = 0, valread;
  struct sockaddr_in serv_addr;

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
  random_item = rand() % (MAX_ITEMS); // Generate random item
  write(sock, &random_item, sizeof(int)); //Client sends the nubmer of the item that he wants
  printf("Number of item %d ,requested for purchase\n", random_item);
  read(sock,&error_flag, sizeof(int)); //Reads the status of the order from server
  if (error_flag == 0){
    printf("Item %d purchase was successfull\n\n", random_item);
  }
  else{
    printf("Item %d purchase was unsuccessfull\n\n", random_item);
    exit(1);
  }
  close(sock);
}

int main(int argc, char const *argv[]) {
  srand(time(NULL)); // Seed the random number generator once at the start

  for (int i = 0; i < MAX_CLIENTS; i++){
    printf("\n\nClient: %d\n\n", i+1);
    for (int y=0 ;y<ORDERS ;y++) {
      pid_t pid = fork();
      random_item = rand() % (MAX_ITEMS); // Generate random item
      if (pid == 0) {//Child process sends order to server
          
        Send_Order(random_item);
        exit(0);
      } 
      else if (pid > 0) {//Parent process adds 1sec delay to the orders
        sleep(1);
      } 
      else {
        perror("Fork failed");
        exit(1);
      }         
    }
  }
  for (int i = 0; i < ORDERS; i++) {//Wait for process to finish
      wait(NULL);
  }
  
return 0;
} 
