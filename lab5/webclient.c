#include<string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<netdb.h>
#include <time.h> 
#include<signal.h>
clock_t st,et;

	
int error(char *msg)
{
	printf("%s\n",msg);
}
int tim=0;
void sig(int status)
{
	st = et - st; 
    double time_taken = ((double)st)/CLOCKS_PER_SEC;
	printf("time taken %f seconds\n",time_taken);
	exit(0);
}
int fetch_response(char *host,char *req) {
	alarm(4);
   int sockfd, portno, n;
	char request[1000];
	sprintf(request,"GET %s HTTP/1.1\r\nHost: %s\r\n\r\n",req,host);
    printf("request: [[%s]]\n",request);
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[4096];

    portno = 80;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(host);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) error("ERROR connecting");

	n = write(sockfd,request,strlen(request));
	st = clock();	 
    if (n < 0) error("ERROR writing to socket");
    while (1) 
	{
		bzero(buffer,4096);
		n = recv(sockfd,buffer,4095, 0);
		if (n < 0) 
		{
			error("ERROR reading from socket");
			break;
		}
		if (n == 0) 
		{
			break;
		}
		  // printf("%d\n", (int)strlen(buffer));
		// printf("%d\n", n);
		et=clock();
		printf("%s",buffer);

	}
	printf("\n");
    close(sockfd);
    return 0;
}
int main(int argc,char *argv[])
{
	signal(SIGALRM,sig);
	if(argc!=2)
	{
		printf("put address as arguments without https:// \n");
		exit(0);
	}
	if((strstr(argv[1],"http://")!=NULL)||(strstr(argv[1],"https://")!=NULL))
	{
		printf("put address as arguments without https:// \n");
		exit(0);
	}
	char x[10000]={'\0'};
	strcpy(x,argv[1]);
	printf("argv[1]: [ %s]\n",x);
	char *pos=strchr(x,'/');
	if(pos!=NULL)
	{
		char *host=x;
		*pos++='\0';
		if(*(pos)=='\0')
		{
			printf("using 3 host :%s req %s\n",host,"/");
	
			fetch_response(host,"/");
			exit(0);
		}
		// printf("pos at %c\n",*pos);
		// exit(0);
		int i=0;
	
		char req[1000]={'\0'};
		req[0]='/';
		for(int i=0;*(pos+i);i++)
		{
			req[i+1]=*(pos+i);
		}
		printf("using 1 host :%s req %s\n",host,req);
		fetch_response(host,req);
		
		
	}
	else
	{
		char *host=argv[1];
		printf("using 2 : host:%s\n",host);
		fetch_response(host,"/");
	}
}
