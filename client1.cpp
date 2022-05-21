#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<iostream>
#include<unordered_map>
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
    
    std::unordered_map<std::string, int> hm;
    //hm.insert(std::make_pair("GETLOG", 1));
    hm["GETLOG"]= 1;
    hm["gETlOG"]= 1;
    hm["Getlog"]= 1;
    hm["getLog"]= 1;
    hm["getLOG"]= 1;
    hm["GeTLoG"]= 1;
    hm["GTlOG"]= 1;
    hm["lOG"]= 1;
    hm["getLLog"]= 1;
    hm["LgetLlog"]= 1;
    hm["gettLOG"]= 1;
    // pointer
    struct tm* ptime;
    // use time function
    
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server; //used to store info about host eg.. hostname, ip etc...
    
    char buffer[255];
    if(argc< 3)
    {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(1);
    }
    portno = atoi(argv[2]);
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
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd< 0)
    {
        error("Error opening socket");
    }
    server= gethostbyname(argv[1]);
    if(server == NULL)
    {
        fprintf(stderr, "Error, no such hosts exist on the network");
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    if(connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0)
    {
        error("Connection Failed");
    }
    while(1)
    {
        bzero(buffer, 255);
        printf("Client: ");
        fgets(buffer, 255, stdin);
        std::string sf="";
        for(int i=0; i<6; i++)
        {
            sf=sf+buffer[i];
        }
        printf("\n");
        auto iter= hm.find(sf);
        if(iter!=hm.end())
        {
            time_t current_time;
            time(&current_time);
            // gets the current-time
            ptime = gmtime(&current_time);
            printf("Server  ( India ) %2d:%02d:%02d : ", (ptime->tm_hour + IND-2) % 24, ptime->tm_min+30, ptime->tm_sec);
            printf("Did you mean 'GetLog'? \n");
            continue;
        }
        n=write(sockfd, buffer, strlen(buffer));
        if(n < 0)
        {   
            error("Error on Writing");
        }
        int i= strncmp("exit", buffer, 4);
        if(i==0)
        {
            break;
        } 
        
        i= strncmp("GetLog", buffer, 6);
        if(i==0)
        {   time_t current_time;
            time(&current_time);
            // gets the current-time
            ptime = gmtime(&current_time);
            printf("Server  ( India ) %2d:%02d:%02d : ", (ptime->tm_hour + IND-2) % 24, ptime->tm_min+30, ptime->tm_sec);
            while(1){
                bzero(buffer, 255);
                n=read(sockfd, buffer, 255);
                printf("%s", buffer);
                if(buffer[n-1]=='~')
                {
                    break;
                }
            }
            printf("\n");
            continue;

        }
        else{
            bzero(buffer, 255);
            n=read(sockfd, buffer, 255);
            if(n<0)
            {
                error("Error on reading");
            }
            else{
                int i= strncmp("exit", buffer, 4);
                if(i==0)
                {
                    break;
                }
                time_t current_time;
                time(&current_time);
                // gets the current-time
                ptime = gmtime(&current_time);
                printf("Server  ( India ) %2d:%02d:%02d : ", (ptime->tm_hour + IND-2) % 24, ptime->tm_min+30, ptime->tm_sec);
                printf("%s\n", buffer);

            }
        }
        
    }
    close(sockfd);
    return 0;
}