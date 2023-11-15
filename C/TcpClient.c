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
	struct sockaddr_in add;
	sd = socket(AF_INET,SOCK_STREAM,0);
	add.sin_family = AF_INET;
	add.sin_addr.s_addr = inet_addr("127.0.0.1");
	add.sin_port = htons(9704);
	
	connect(sd,(struct sockaddr *)&add,sizeof(add));
	printf("connected");
	
	printf("enter message to server:");
	gets(buf);
	int m = write(sd,buf,sizeof(buf));
	int n1 = read(sd,buf,sizeof(buf));
	puts(buf);

}
