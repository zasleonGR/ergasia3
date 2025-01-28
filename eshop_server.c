#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>


//Global variables
double total_profit = 0.0;
int sucs_orders = 0;
int failed_orders = 0;
double total_price; 
int error_flag;
int sucs_request;
int failed_request;
int item_number;
int not_complete_orders;


int main() {  
    srand(time(NULL));
    int opt=1; //Used in line 109
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
 
    initialize_catalog(); //We initalize the catalog (and the prices of the items etc)
    initialize_prices();

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }
    //It is used to prevent the error Binding failed:Address already in use
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    
    // Set up address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    // Build the socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    
    printf("Eshop is waiting for orders...\n\n");
    
    for (int i = 0; i < MAX_CLIENTS; i++){
        printf("Client: %d\n",i+1);
        for (int j = 0; j < ORDERS; j++){  
            // Accept connection
            if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
                perror("Accept failed");
                close(server_fd);
                exit(EXIT_FAILURE);
            }
            // Read and respond to client
            read(new_socket, &item_number, sizeof(int)); //Server reads the item number the client requested
            printf("Received item number %d for order\n", item_number);
            process_order(item_number);
            write(new_socket, &error_flag, sizeof(int));//Server sends the result of the order   
        close(new_socket);
        }
        print_result(i+1, total_price, error_flag,not_complete_orders);
        not_complete_orders=0;
    } 
    end_results_print(sucs_orders, failed_orders, total_profit, sucs_request, failed_request);
    
    return 0;
}
