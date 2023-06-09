// concurrent - connectionless server using Fork and process
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>

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
     printf("\tChecking for Duplicate data\n");
     int feed = checkDuplicate(serial_n, reg_n);
     if (feed){
         printf("\tDuplicate data entered: \n");
         strcpy(status, "Duplicate Error: ither serial number or registration number is a duplicate");
     } else {
       printf("\tWriting client data to text file\n");
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

#define PORT 8080
#define MAX_MSG_LEN 1024

int main() {
    int server_fd, len;
    char buffer[MAX_MSG_LEN];
    struct sockaddr_in server_addr, client_addr;
    char* substrings[3];
    char status[1000];
    printf("\n- Creating server socket\n");
    //create a socket and bind to an address
    server_fd = socket(AF_INET, SOCK_DGRAM, 0);
    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    printf("- Binding server socket to server-address\n");
    bind(server_fd, (const struct sockaddr *)&server_addr, sizeof(server_addr));
    printf("- Enter a continuous loop to handle client requests\n");

    int count = 1;
        while (1) {

            printf("\n\n\tWaiting for client data...\n");

            len = sizeof(client_addr);
            memset(buffer, 0, sizeof(buffer));

            recvfrom(server_fd, buffer, MAX_MSG_LEN, 0, (struct sockaddr *)&client_addr, &len);
            printf("\tProcess %d is runnig\n\n", count );
            printf("\tReceived data from the client\n");


            int pid = fork(); // Fork a child process for each client
            if (pid == 0) {
                printf("\tProcess %d is sleeping\n\n\n", count );
                sleep(10);
                printf("\tProcess %d is runnig\n\n", count );
                
                printf("\tChild process created\n");
                printf("\tAnalyzing client's data\n");

                char* substrings[3];
                splitStringByComma(buffer, substrings, 3);

                printf("\n\t   Client data received:");
                printf("\n\t\t1. Serial Number        : %s", substrings[0]);
                printf("\n\t\t2. Registration Number  : %s", substrings[1]);
                printf("\n\t\t3. Client Name          : %s", substrings[2]);
                printf("\n\n");

                char* status = write_to_text_file(substrings[0], substrings[1], substrings[2]);

                printf("\tSending data to the client\n");
                sendto(server_fd, status, strlen(status), 0, (struct sockaddr *)&client_addr, len); // Send message back to the client

                printf("\tChild process completed. Exiting.\n");
                printf("\tProcess %d Finished\n\n", count );
                exit(EXIT_SUCCESS);

            }
            count = count + 1;
        }


    return 0;
}
