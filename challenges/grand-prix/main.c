#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "coordinates.h"

// Default message to be sent to clients
char serverMessage[] = "Message X";
int numRacers = 0;
int numLaps = 0;

void clearMessage(){
    for(int i = 0; i < 10; ++i){
        serverMessage[i] = 0;
    }
}

static void * connectionThread(void *arg){
    char *s = (char *) arg;
    printf("%s\n", s);

    char msgIdx = '0';

    // create a socket
    int serverSocket;
    serverSocket = socket(AF_INET, SOCK_STREAM, 0); // Using default protocol (TCP)

    // define the server address
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(9009);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to our specified IP and port
    bind(serverSocket, (struct sockaddr*) &serverAddress, sizeof(serverAddress));

    // Listen for connections
    listen(serverSocket, 1);

    // Handle connections
    int clientSocket = accept(serverSocket, NULL, NULL);

    int cmd = getchar();
    while(cmd != 'q'){
        serverMessage[8] = msgIdx;
        // Send Single Message
        send(clientSocket, serverMessage, sizeof(serverMessage), 0);
        msgIdx++;
        char serverResponse[64];
        recv(clientSocket, &serverResponse, sizeof(serverResponse), 0);
        printf("From Raspberry: %s", serverResponse);
        cmd = getchar();
        if (cmd == 'q'){
            clearMessage();
            stpcpy(serverMessage, "Quit");
            send(clientSocket, serverMessage, sizeof(serverMessage), 0);
            printf("%s\n", serverMessage);
            break;
        }
    }

    printf("Closing Socket\n");

    // Close the socket
    close(serverSocket);

    return (void *) strlen(s);
}

static void * blueThread(void *arg){
    char *s = (char *) arg;
    int i;
    int j;
    printf("%s\n", s);

    while(1){
        for(i = 0; i < numLaps; ++i){
            for (j = 0; j < 107; ++j){
                usleep(100000);
                // TODO: Check if corresponding file exists; If (file exist) -> Wait ... else write coordinates
                printf("X: %d\tY: %d\n", inner[j][i], inner[j][i+1]);
                FILE *file;
                file = fopen("status.txt", "w");
                if(file == NULL){
                    printf("Error while opening the status file\n");
                    continue;
                }
                fprintf(file, "%s", "Blue ");
                fprintf(file, "%d", inner[j][0]);
                fprintf(file, "%s", " ");
                fprintf(file, "%d", inner[j][1]);
                if(inner[j][1] == 60){
                   fprintf(file, "%s", " 45"); 
                } else {
                    fprintf(file, "%s", " 0");
                }
                fclose(file);

            }
        }
        break;
    }

    return (void *) strlen(s);
}

void getRacers(){
    while(numRacers == 0){
        printf("Type the number of Racers (1 - 3)\n");
        int num = getchar();
        switch (num) {
            case '1':
                numRacers = 1;
                break;
            case '2':
                numRacers = 2;
                break;
            case '3':
                numRacers = 3;
                break;
            default:
                printf("ERROR: Only numbers from 1 to 4\n\n");
                break;
        }
    }
    getchar();
}

void getLaps(){
    while(numLaps == 0){
        printf("Type the number of Laps (1 - 4)\n");
        int laps = getchar();
        switch (laps) {
            case '1':
                numLaps = 1;
                break;
            case '2':
                numLaps = 2;
                break;
            case '3':
                numLaps = 3;
                break;
            case '4':
                numLaps = 4;
                break;
            default:
                printf("ERROR: Only numeric values from 1 to 4\n\n");
                break;
        }
    }
    getchar();
}

int main() {
    int initFile;
    char initialRacers[1], initialLaps[1];
    printf("Welcome to Grand Prix!\n");
    getRacers();
    getLaps();

    // TODO: Create a Thread for each numRacers

    printf("Racers: %d\nLaps: %d\n", numRacers, numLaps);

    //Init file for python
    initFile = open("initFile.txt", O_WRONLY);
    if (initFile == -1){
    printf("Error creating file");
    }
    sprintf(initialRacers, "%d", numRacers);
    sprintf(initialLaps, "%d", numLaps);
    write(initFile, initialRacers, sizeof(initialRacers));
    write(initFile, initialLaps, sizeof(initialLaps));
    close(initFile);

    pthread_t serverThread;
    pthread_t blueCarThread;
    void *res;
    int s;

    s = pthread_create(&serverThread, NULL, connectionThread, "Initializing TCP Server Thread");
    if (s != 0){
        printf("Error while creating thread\n");
    }

    system("python graphics.py &");

    s = pthread_create(&blueCarThread, NULL, blueThread, "Running Blue Car Thread");
    if (s != 0){
        printf("Error while creating thread\n");
    }

    s = pthread_join(serverThread, &res);
    if (s != 0){
        printf("Error while joining thread\n");
    }

    printf("Thread Joined\n");

    return 0;
}