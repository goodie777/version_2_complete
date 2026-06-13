#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> 

#include "structures.h"



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

    // ADD THESE TWO LINES
    int opt = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

   

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

    //accept
    while (1) {
      int cfd = accept(fd, (struct sockaddr*)&clientInfo, &clientSize);
      if (cfd == -1) { perror("accept"); }
      TC_101 telecommand = {0};
      recv(cfd, &telecommand, sizeof(telecommand), 0);
      TM_201 data4 = {201};
      data4.cpu_usage = 20.1;
      data4.memory_usage = 50.8;
      data4.battery_level = 90.8;
      send(cfd, &data4, sizeof(data4), 0);
      printf("Client says: %d\n", telecommand.command_id );
      close(cfd);
      
   }

      return 0;


    

