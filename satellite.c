#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> 
#include <string.h>

#include "structures.h"

typedef struct recv_generic_tc {
    uint32_t command_id;
} recv_generic_tc;

// Create the socket 
int main(void) {
    struct sockaddr_in serverInfo = {0};
    struct sockaddr_in clientInfo = {0};
    socklen_t clientSize = sizeof(clientInfo);
    
    serverInfo.sin_family = AF_INET;
    serverInfo.sin_addr.s_addr = 0;
    serverInfo.sin_port = htons(5555);
    
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        perror("socket");
        return -1;
    } 

    // // ADD THESE TWO LINES
    // int opt = 1;
    // setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
   

    //bind
    if (bind(fd, (struct sockaddr*)&serverInfo, sizeof(serverInfo)) == -1) {
        perror("bind");
        close(fd);
        return -1;  
    }

    //listen
    if (listen(fd, 0) == -1) {
        perror ("listen");
        close(fd);
        return -1;
    }

    int cfd = accept(fd, (struct sockaddr*)&clientInfo, &clientSize);

    // Create the buffer just to read the data in, needs to be as large as our largest possible TC/TM we expect, right now we're asking for a load more 
    char buff[1024] = {0};

    //accept
    while (1) {
      // Zero out the buffer first to read the data into 
      bzero(buff, sizeof(buff));

      if (cfd == -1) { perror("accept"); }

      // Read the 1024 bytes of data into the buffer, wait for the data
      ssize_t n = recv(cfd, (char *)buff, sizeof(buff), 0);

    // Kernel Socket Buffer
    // +---+---+---+---+
    // |101| 0 | 0 | 0 |
    // +---+---+---+---+
    //           |
    //           | copy
    //           v
    // Your Process Memory
    // buffer
    // +---+---+---+---+
    // |101| 0 | 0 | 0 |
    // +---+---+---+---+


      buff[n]  = '\0'; // Null-terminate the buffer to make it a valid string

      // Now we want to parse what's actually in this buffer, so we can cast it to our telecommand struct
      // Here we're saying that the buffer has the TC_101 structure, so we can reference as required
      uint32_t command_id = *(uint32_t *)buff;

      printf("Client says: %d\n", command_id );

      // Not needed yet but do different things based on the command_id
      switch (command_id)
      {
      case 101:
        /* code for TC_101 */
        TC_101* tc_101 = (TC_101*)buff;
        printf("Received TC_101 with command_id: %d\n", tc_101->command_id);
        break;
      case 102:
        /* code for TC_102 */
        TC_102* tc_102 = (TC_102*)buff;
        printf("Received TC_102 with command_id: %d, DeltaVx: %f, DeltaVy: %f, DeltaVz: %f\n", tc_102->command_id, tc_102->DeltaVx, tc_102->DeltaVy, tc_102->DeltaVz);
        break;
      case 103:
        /* code for TC_103 */
        TC_103* tc_103 = (TC_103*)buff;
        printf("Received TC_103 with command_id: %d, operation_code: %d, payload_id: %d\n", tc_103->command_id, tc_103->operation_code, tc_103->payload_id);
        break;

      default:
        break;
      }

      // Send back a generic TM_201 response to the ground station
      TM_201 data4 = {201};
      data4.cpu_usage = 20.1;
      data4.memory_usage = 50.8;
      data4.battery_level = 90.8;
      send(cfd, &data4, sizeof(data4), 0);
      printf("Send TM back \n");
    }
    
    close(cfd);
    
    return 0;
}
