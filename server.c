/* 
* Shih-Chieh Lin
* CSCE 5580 pogramming assignment 3
* 4//2017
* A server (cse01.cse.unt.edu)in the internet using TCP domain, and the port number is get from argument number 1
* Can calculate the numerical expression from client and write the result back to display on client.
* 
* Reference:
*/

#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>

void error(char *msg)
{
    perror(msg);
    exit(1);
}

int isDigit(char p){//check digit
  if (p>='0' && p<='9'||p=='e'){return 1;}
  else{return 0;}
}

int ise1(char p){//check e
  if (p=='e'){return 1;}
  else{return 0;}
}

int isNumber(char *p){//check if is number
  while(*p){
    if(!isDigit(*p))
    {return 0;}
    p++;
  }
  return 1;
}
/*
char getPrintableChar(void)
{
  char temp;
  do{temp = getchar();}	
  while (isblank(temp));
  return temp;
}
*/
int isLeftParenthesis(char p){//check left parenthesis
  if (p=='('){return 1;}
  else
  {return 0;}
}

int isRightParenthesis(char p){//check right parentesis
  if (p==')'){return 1;}
  else
  {return 0;}
}

int isOperator(char p){//check what kind of operator
  if(p=='+' || p=='-' || p=='*' || p=='/' || p=='^') {return p;}
  else
  {return 0;}
} 

double perform(double x,double y,char p){
  switch(p){
    case '+':
	printf("sum%f\n",x+y);
	return x+y;
	break;
    case '-': 
	printf("mi%f\n",x-y);
	return x-y;
	break;
    case '*': 
	printf("pro%f\n",x*y);
	return x*y;
	break;
    case '/': 
        if(y=='0'){printf("Invalid expression\n");break;}
        else {return (double)x/y;}
	break;
    case '^': 
	return pow(x,y);
	break;
  }
  return 0;
}

int isEmpty(char *p){
  if((int)strlen(p)==0){return 1;}
  else{return 0;}
}

int ismd(char *p){
  for(;*p;p++){
    if(!isDigit(*p) && *p!='*'&& *p!='/'){return 0;}
  }
  return 1;
}

int ise(char *p){
  for(;*p;p++){
    if(ise1(*p)){return 1;}
  }
  return 0;
}

int getleftoperand(char *p, char *l){
  int i=0;
  if (ismd(p)){
    while(1){
      if(*p =='*' || *p =='/')
      {break;}
      l[i++]=*p++;
    }
    return i;
  }
  if(isLeftParenthesis(*p)){
    int rightParnum=0;
    int leftParnum=1;
    p++;
    while(1){
      if (isLeftParenthesis(*p)){leftParnum++;}
      if (isRightParenthesis(*p)){rightParnum++;}
      if (isRightParenthesis(*p) && leftParnum==rightParnum){break;}
      l[i++]=*p++;
    }
    l[i]='\0';
    return i+2;
  }
  while(1){
    if(!isDigit(*p)) {break;}
    l[i++]=*p++;
    //printf("have e!%d!\n",l[i]);
  }
  l[i]='\0';
  return i;
}

int getoperator(char *p, int index, char *operator){
  *operator=p[index];
  return index+1;
}

int getrightoperand(char *p, char *l){
  while(*p && (isDigit(*p) || isOperator(*p)|| isLeftParenthesis(*p)|| isRightParenthesis(*p))){
    *l++=*p++;}
  *l='\0';
  return 0;
}

double calculate(char *p){
  char operator;
  char leftoperand[256]="";
  char rightoperand[256]="";
  double temp;
  do{
    if (isNumber(p))
    {return atof(p);}
    //else if (isEmpty(p))
    //{printf("ERROR!\n");}
  }while(p=='\0');
  //printf("iseturn!%s\n",p);
  int leftopindex = getleftoperand(p,leftoperand);
  //printf("left%d\n",leftopindex);
  //printf("et%d\n",leftoperand);
  //printf("ft%s\n",leftoperand);
  int operatorindex = getoperator(p,leftopindex,&operator);
  //printf("op%d\n",operatorindex);
  int rightopindex = getrightoperand(p+operatorindex,rightoperand);
  //printf("rig%d\n",rightopindex);
  //printf("rt%s\n",rightoperand);
  double k;
  if (isEmpty(rightoperand))
  {
    k=calculate(leftoperand);
    //return -1;//Invalid expression
    return calculate(leftoperand);
  }
//calculate e
  if(!strcmp(leftoperand,"e")){
    return perform(exp(1),calculate(rightoperand),operator);}
  else if(!strcmp(rightoperand,"e")){
    return perform(calculate(leftoperand),exp(1),operator);}
  else  
  {return perform(calculate(leftoperand),calculate(rightoperand),operator); }
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno, clilen,pid;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     if (argc < 2) {
       fprintf(stderr,"ERROR, no port provided\n");
       exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
     {error("ERROR opening socket");}
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
     {error("ERROR on binding");}
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
     if (newsockfd < 0) 
     {error("ERROR on accept");}
     while(1){
       bzero(buffer,256);
       n = read(newsockfd,buffer,255);
       if (n < 0) {error("ERROR reading from socket");}
       //printf("Here is the message: %s\n",buffer);
       if(buffer[0]=='q' && buffer[1]=='u' && buffer[2]=='i'&& buffer[3]=='t'){//quit the program
         n= write(newsockfd,"bye bye!",8);
         close(newsockfd);
         return 0;
       }
       else{
         double num=calculate(buffer);
         //if(num==-1){n=write(newsockfd,"Invalid expression",18);}
	 printf("num%.4f\n",num);
         //if(num==-1){n=write(newsockfd,"Invalid expression",18);}
         //else{
           sprintf(buffer,"%.4f",num);//itoa
	   printf("result: %s\n", buffer);
	   n=write(newsockfd,buffer,strlen(buffer));
         //}
       }
       if (n < 0) {error("ERROR writing to socket");}
     }
     close(newsockfd);
     return 0; 
}
