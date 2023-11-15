#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>


void main()
{
	int sd;
	char buf[25];
	struct sockaddr_in sadd,cadd;
	sd = socket(AF_INET,SOCK_STREAM,0);
	sadd.sin_family = AF_INET;
	sadd.sin_addr.s_addr = inet_addr("127.0.0.1");
	sadd.sin_port = htons(9704);
	
	int n =  bind(sd,(struct sockaddr*)&sadd,sizeof(sadd));
	if(n<0)
	{
		printf("not binded");
		exit(1);
	}
	printf("binded");
	int p = listen(sd,5);
	
	
	printf("listening");
	while(1)
	{
	int clilen = sizeof(cadd);
	int newsd = accept(sd,(struct sockaddr *)&cadd,&clilen);
	
	int n1 = read(newsd,buf,sizeof(buf));
	puts(buf);
	printf("enter message to server:");
	gets(buf);
	int m = write(newsd,buf,sizeof(buf));
	}
	

}
