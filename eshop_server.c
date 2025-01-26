#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define CLIENTS 5       
#define MAX_ITEMS 20
#define ORDERS 10

double total_profit = 0.0;
int sucs_orders = 0;
int failed_orders = 0;
char Buffer[256] = { 0 };
int random_item;
double total_price; 
int error_flag;
int sucs_request;
int failed_request;

struct Product {
    char description[100];
    double price;
    int item_count;
};

struct Product catalog[MAX_ITEMS]; //initialization of the catalog


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




int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    srand(time(NULL));
    
    initialize_catalog(); //We initalize the catalog (and the prices of the items etc)
    initialize_prices();

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
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

    printf("Server is listening on port 8080...\n");

    for (int i=0; i<CLIENTS; i++) {
        printf("Client %d is waiting for connection...\n", i+1);
    // Accept connection
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("Accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Read and respond to client
    read(new_socket, buffer, 1024);
    printf("Received: %s\n", buffer);
        
    send(new_socket, "hello", strlen("hello"), 0);
    printf("Responded to client %d\n",i+1);
        
    }

    // Cleanup
    close(new_socket);
    close(server_fd);

    return 0;
}
