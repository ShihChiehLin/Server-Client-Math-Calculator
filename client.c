/*
* Shih-Chieh Lin
* CSCE 5580 pogramming assignment 1
* 2/12/2017
* A client (cse02.cse.unt.edu) in the internet using TCP domain, and need to enter port number as argument number 1
* Can connect to math server (cse01..cse.unt.edu)and enter the numerical expression the user want to calculate and display the result of it from the math server.
* Not finish:
  check space between parenthesis
  check space in right operand
  sqrt and log function calculate
  negative number check
* Reference:http://www.linuxhowtos.org/C_C++/socket.htm
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void getInput(char *input){
  printf("exprsn: ");
  fgets(input,256, stdin);
}

void error(char *msg){
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[]){
    int sockfd, portno, n,m;
    int v;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];
    if (argc < 2) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[1]);//port number from input
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    inet_pton(AF_INET,"129.120.151.94",&(serv_addr.sin_addr));//cse01.cse.unt.edu
    serv_addr.sin_port = htons(portno);//port number
    if ((connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr))) < 0)
    {error("ERROR connecting");}
    while(1){
      bzero(buffer,256);
      getInput(buffer);
      //printf("cget: %s\n",buffer);
      n = write(sockfd,buffer,strlen(buffer));
      //printf("cw: %s\n",buffer);
      if (n < 0) 
      {error("ERROR writing to socket");}
      bzero(buffer,256);
      m = read(sockfd,buffer,255);
      printf("result: %s\n",buffer);
      if (m < 0) 
      {error("ERROR reading from socket");}
      if (strncmp(buffer, "bye bye!", 8) == 0) {break;}
    }
    close(sockfd);
    return 0;
}
