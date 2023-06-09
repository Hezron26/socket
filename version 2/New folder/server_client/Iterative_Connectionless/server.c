//  Iterative, Connectionless server
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 8080

int checkDuplicate(char serialNumber[], char regNumber[]) {
    FILE *file;
    char line[100];
    char tempSerialNumber[100], tempRegNumber[100];
    file = fopen("data.txt", "r"); // open file in read mode
    if (file == NULL) {
        return 0; // return 0 if file cannot be opened
    }
    while (fgets(line, 100, file) != NULL) {
        sscanf(line, "%s %s", tempSerialNumber, tempRegNumber); // read serial number and registration number from file
        if (strcmp(tempSerialNumber, serialNumber) == 0 || strcmp(tempRegNumber, regNumber) == 0) {
            fclose(file);

            return 1; // return 1 if either serial number or registration number is a duplicate
        }
    }
    fclose(file);
    return 0; // return 0 if neither serial number nor registration number is a duplicate
}

char* write_to_text_file(char serial_n[], char reg_n[], char name[]){
     char* status = malloc(sizeof(char) * 110);
     printf("\tChecking for duplicates \n");
     int feed = checkDuplicate(serial_n, reg_n);
     if (feed){
         printf("\tDuplicate data entered\n");
         strcpy(status, "Duplicate Error: ither serial number or registration number is a duplicate");
     } else {
       printf("\tWriting to client data to text file \n");
       FILE *file;
       file = fopen("data.txt", "a"); // open file in append mode
       fprintf(file, "%s %s %s\n", serial_n, reg_n, name); // write details to file
       fclose(file);
       printf("\tDetails saved to file.\n");
       strcpy(status, "Details saved to file");
     }
     return status;
}

int splitStringByComma(char* str, char** substrings, int maxSubstrings) {
    int numSubstrings = 0;
    char* token;
    token = strtok(str, ",");
    while (token != NULL && numSubstrings < maxSubstrings) {
        substrings[numSubstrings++] = token;
        token = strtok(NULL, ",");
    }
    return numSubstrings;
}


int main() {
    int server_fd, valread;
    struct sockaddr_in address, client_addr;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char* substrings[3];

    printf("\n\n- Create server socket\n");

    // Create a socket and bind to well known address
    server_fd = socket(AF_INET, SOCK_DGRAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    printf("- Binding server socket to server addresss\n");
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));

    printf("- Entering a continuous loop to handle client requestst\n");

    // repeatedly read the next request from client, formulate a respose and send a reply back to the client
    while (1) {

        recvfrom(server_fd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &addrlen); //read request from client
        printf("\n\n\treceiving request from client\n");

        printf("\tSplit the received message into substrings using commas as separator\n");

        splitStringByComma(buffer, substrings, 3);

        printf("\n\t   client data Received  ");
        printf("\n\t\t1. Serial Number        : %s",substrings[0]);
        printf("\n\t\t2. Registration Number  : %s",substrings[1]);
        printf("\n\t\t3. Client Name          : %s",substrings[2]);
        printf("\n\n     ");

        char* status = write_to_text_file(substrings[0], substrings[1], substrings[2] );

        printf("\tSending data to client\n");

        sendto(server_fd, status, strlen(status),  0,  (struct sockaddr *)&client_addr, addrlen); //reply back to the client

        printf("\tClear the buffer \n");

        memset(buffer, 0, sizeof(buffer));  // sets all the bytes in the buffer variable to zero.


    }

    return 0;
}
