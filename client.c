#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/socket.h> // AF_INET and SOCK_STREAM
#include <arpa/inet.h> // storage size of serv_addr
#include <unistd.h> // getpid, getppid, fork
#include <string.h> // bzero
#include <time.h>


//Global variables
int random_item;
int error_flag;
int random_item;


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
