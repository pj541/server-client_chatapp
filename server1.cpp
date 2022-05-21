#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<iostream>
#include<fstream>

#include <time.h>
#define CST (+8)
#define IND (-5)



void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    //if user does not provide any port information...
    if(argc < 2)
    {
        fprintf(stderr, "Port Number not provided. Program terminated \n");
        exit(1);
    }
    

    // pointer
    struct tm* ptime;
    // use time function
    //time(&current_time);
    // gets the current-time
    //ptime = gmtime(&current_time);
    int sockfd, newsockfd, portno, n;
    char buffer[255];
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;
    sockfd= socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd<0)
    {
       error("Error opening socket. ");
    }
    //bzero is used to clear any data in a pre existing var just so we can store the clean data
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno= atoi(argv[1]);
    if(portno >=0 && portno<=1023)
    {
        fprintf(stderr, "You are using a reserved port, kindly use unreserved ! Program Terminated. \n");
        exit(1);
    }
    else if(portno<0)
    {
        fprintf(stderr, "Port Value can't be negative. Program Terminated. \n");
        exit(1);
    }
    //get serv_addr
    
    serv_addr.sin_family= AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    
    if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))< 0)
    {
        error("Binding Failed");
    }
    
    listen(sockfd, 5);
    clilen= sizeof(cli_addr);
    
    newsockfd= accept(sockfd, (struct sockaddr*) &cli_addr, &clilen);
    
    if(newsockfd< 0)
    {
        error("Client limit reached");
    }
    
    while(1)
    {
        bzero(buffer, 255);
        n= read(newsockfd, buffer, 255);
        if(n < 0)
        {
            error("Error on reading");
        }
        int k= strncmp("GetLog", buffer, 6);
        if(k==0)
        {   bzero(buffer, 255);
            time_t current_time;
            time(&current_time);
            // gets the current-time
            ptime = gmtime(&current_time);
            std::ifstream dataFile("somefile1.txt");
            printf("Server  ( India ) %2d:%02d:%02d : ", (ptime->tm_hour + IND-2) % 24, ptime->tm_min+30, ptime->tm_sec);
            printf("The log file has been sent \n\n");
            while(dataFile.getline(buffer, 255))
            {     
                  n=write(newsockfd, buffer, strlen(buffer));
                  bzero(buffer, 255);
            }
            dataFile.close();
            buffer[0]='~';
            n=write(newsockfd, buffer, strlen(buffer));
            bzero(buffer, 255);

            continue;
        }
        else{
            k= strncmp("exit", buffer, 4);
            if(k==0)
            {
                bzero(buffer, 255);
                time_t current_time;
                time(&current_time);
                // gets the current-time
                ptime = gmtime(&current_time);
                printf("Server  ( India ) %2d:%02d:%02d : ", (ptime->tm_hour + IND-2) % 24, ptime->tm_min+30, ptime->tm_sec);
                printf("Client Disconnected\n");
                close(newsockfd);
                close(sockfd);
                sockfd= socket(AF_INET, SOCK_STREAM, 0);
                if(sockfd<0)
                {
                   error("Error opening socket ");
                }
                //bzero is used to clear any data in a pre existing var just so we can store the clean data
                bzero((char *) &serv_addr, sizeof(serv_addr));
                portno= atoi(argv[1]);

                //get serv_addr
                serv_addr.sin_family= AF_INET;
                serv_addr.sin_addr.s_addr = INADDR_ANY;
                serv_addr.sin_port = htons(portno);

                if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))< 0)
                {
                    error("Binding Failed");
                }
                listen(sockfd, 5);
                clilen= sizeof(cli_addr);
                newsockfd= accept(sockfd, (struct sockaddr*) &cli_addr, &clilen);
                continue;

            }else{
                time_t current_time;
                time(&current_time);
                // gets the current-time
                ptime = gmtime(&current_time);
                printf("Client  ( India ) %2d:%02d:%02d : ", (ptime->tm_hour + IND-2) % 24, ptime->tm_min+30, ptime->tm_sec);
                printf("%s\n", buffer);
                }
        }

        bzero(buffer, 255);
        //fgets functions reads bytes to a stream
        //std::cout << "\033[2J\033[1;1H";
        printf("Server: ");
        fgets(buffer, 255, stdin);
        printf("\n");
        n= write(newsockfd, buffer, strlen(buffer));
        if(n< 0)
        {
            error("Error on Writing");
        }
        int i= strncmp("exit" , buffer, 4);
        if(i==0)
        {
            break;
        }
    }
    close(newsockfd);
    close(sockfd);
    return 0;
}