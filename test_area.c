//filename is test_area.c

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>


#include "structures.h"





int main()
{
    int fd, cfd;
    struct sockaddr_in serverAddr, clientInfo;
    socklen_t clientSize = sizeof(clientInfo);

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        perror("socket");
        return 1;
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(5001);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

     

    if (bind(fd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("bind");
        return 1;
    }

    if (listen(fd, 5) == -1) {
        perror("listen");
        return 1;
    }

    printf("Server listening on port 5001...\n");

    while (1) {

        cfd = accept(fd, (struct sockaddr *)&clientInfo, &clientSize);
        if (cfd == -1) {
            perror("accept");
            continue;
        }

        printf("[SATELLITE] Ground Station connected.\n");

        uint32_t command_id;

        if (recv(cfd, &command_id, sizeof(command_id), 0)
            != sizeof(command_id)) {
            fprintf(stderr, "Failed to receive command_id\n");
            close(cfd);
            continue;
        }

        printf("[SATELLITE] Received TC_%u\n", command_id);
        printf("[SATELLITE] Waiting for Telecommand from Ground Station...\n");

        if (command_id == 101) {

            TM_201 tm = {
                .status_code = 201,
                .cpu_usage = 15.0f,
                .memory_usage = 30.0f,
                .battery_level = 80.0f
            };
            
            printf("[SATELLITE] Sending TM_201 to Ground Station...\n");
            send(cfd, &tm, sizeof(tm), 0);
            
        }

        else if (command_id == 102) {

            TC_102 tc;

            recv(cfd, &tc, sizeof(tc), 0);

            TM_202 tm = {
                .status_code = 202,
                .current_altitude = 20.1f,
                .velocity_vector = {10.5f, 20.5f, 30.5f}
            };
            printf("[SATELLITE] Sending TM_202 to Ground Station...\n");
            send(cfd, &tm, sizeof(tm), 0);
    
        }

        else if (command_id == 103) {

            struct {
                uint32_t command_id;
                uint32_t operation_code;
                uint32_t payload_id;
            } tc;

            recv(cfd, &tc, sizeof(tc), 0);

            TM_203 tm = {
                .status_code = 203,
                .payload_id = tc.payload_id,
                .operational_status = 1,
                .data_measurements = {7.7f, 9.9f, 11.7f}
            };
            printf("[SATELLITE] Sending TM_203 to Ground Station...\n");
            send(cfd, &tm, sizeof(tm), 0);
        }

        close(cfd);
        printf("[SATELLITE] Waiting for next Telecommand...\n");
    }

    return 0;
}