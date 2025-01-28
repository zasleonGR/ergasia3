#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/socket.h> 
//Constast variables

#define MAX_CLIENTS 5       
#define MAX_ITEMS 20
#define ORDERS 10
#define PORT 8080


void print_result(int Client, double Final_price, int flag,int n_c_o) {
    printf("Client %d:", Client);
    total_profit += Final_price; // Add to total_profit for this client
    if (flag == 0 && n_c_o ==0) {
        printf(" Purchase complete, your total is: %.2f euro.(Successful!!!)\n\n", Final_price);
        sucs_request++;    
    } else {
        printf(" Purchase complete,%d products were unavailable, your total is: %.2f euro.(Failed!!!)\n\n", not_complete_orders,Final_price);
        failed_request++;\  
    }
}

void end_results_print(int s_o,int f_o,double t_p,int s_r,int f_r){
    printf("\nTotal orders: %d\n", sucs_orders + failed_orders);
    printf("Total successful orders: %d\n", sucs_orders);
    printf("Total failed orders: %d\n", failed_orders);
    printf("Total profit: %.2f euro.\n\n", total_profit);
    printf("%d products were requested,where %d products were bought,totaling %.2f euros.\n", sucs_orders + failed_orders, sucs_orders, total_profit);
    printf("%d requests were made,where %d succeeded and %d failed\n", sucs_request+failed_request, sucs_request, failed_request);
}


void initialize_catalog() {
    for (int i = 0; i < MAX_ITEMS; i++) {
        catalog[i].item_count = 2;
    }
}

void initialize_prices() {
    for (int i = 0; i < MAX_ITEMS; i++) {
        catalog[i].price = rand() % (1000 + 1);
    
    }
}

int process_order(int item_number) {
    sleep(0.5);//Order takes 0.5secs to be proccsed
    if (catalog[item_number].item_count > 0) {
        catalog[item_number].item_count--; 
        total_price = catalog[item_number].price; 
        error_flag = 0; // Order successful
        sucs_orders++;
    } else {
        error_flag = 1; // Order failed
        not_complete_orders++;
        failed_orders++ ;  
    }
    return error_flag;
}

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
