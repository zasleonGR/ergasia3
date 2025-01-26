#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h> // AF_INET and SOCK_STREAM
#include <arpa/inet.h> // storage size of serv_addr
#include <unistd.h> // getpid, getppid, fork
#include <string.h> // bzero
#include <time.h>
#define PORT 8080
//Constast variables

#define CLIENTS 5       
#define MAX_ITEMS 20
#define ORDERS 10

//Global variables
double total_profit = 0.0;
int sucs_orders = 0;
int failed_orders = 0;
char Buffer[256] = { 0 };
int random_item;
double total_price; 
int error_flag;
int sucs_request;
int failed_request;
int random_item;

void order_update(int e_f){
  if (e_f == 0){
    printf("Item %d purchase was successfull\n", random_item);
  }
  else{
    printf("Item %d purchase was unsuccessfull\n", random_item);
  }
}


int main(int argc, char const *argv[]) {
  srand(time(NULL)); // Seed the random number generator once at the start
 
  
  
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
    for (int i ;i<ORDERS ;i++) {
    sleep(1);
      
    random_item = rand() % (MAX_ITEMS); // Generate random item
      
    
      
    write(sock, &random_item, sizeof(int)); //Clinet sends the number of the wanted item
      
    read(sock,&error_flag, sizeof(int)); //Reads the response from the sever
      
    order_update(error_flag);
    
    
  }
return 0;
} 
