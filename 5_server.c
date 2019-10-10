#include <stdio.h>
#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <zconf.h>
#include <errno.h>

#define SERVER_PORT 12345

ssize_t readn(int, void *, size_t);

void read_data(int);

int main(int argc, char **argv) {
    int listenfd, connfd;
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERVER_PORT);

    bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
    listen(listenfd, 1024);

    for (;;) {
        clilen = sizeof(cliaddr);
        connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen);
        read_data(connfd);
        close(connfd);
    }
    return 0;
}
void read_data(int sockfd) {
    ssize_t n;
    char buf[1024];

    int time = 0;
    for (;;) {
        fprintf(stdout, "block in read\n");
        if ((n = readn(sockfd, (void *) buf, (size_t) 1024)) == 0)
            return;
        time++;
        fprintf(stdout, "1K read for %d \n", time);
        sleep(1);
    }
}

ssize_t readn(int fd, void *vptr, size_t size) {
    size_t nleft;
    ssize_t nread;
    char *ptr;

    ptr = vptr;
    nleft = size;

    while (nleft > 0) {
        if ((nread = read(fd, ptr, nleft)) < 0) {
            if (errno == EINTR)
                nread = 0;
            else
                return (-1);
        } else if (nread == 0) {
            break;
        }

        nleft -= nread;
        ptr += nread;
    }
    return size - nleft;
}

/*
#include "lib/common.h"
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <sys/un.h>
void read_data(int sockfd)
{
	ssize_t n;
	char buf[1024];
	int time = 0;
	for(;;)
	{
		fprintf(stdout,"block in read \n");
		if((n = readn(sockfd,buf,1024)) == 0)
			return;
		
		time++;
		fprintf(stdout,"1k read for %d \n",time);
		usleep(1000);
	}
}

int main(int argc;char **argv)
{
	int listenfd,connfd;
	socklen_t clilen;
	struct sockaddr_in cliaddr,servaddr;
	
	listenfd = socket(AF_INET,SOCK_STREAM,0)
	
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_add.s_addr = htonl(INADDR_ANY); 
	servaddr.sin_port = htons(12345);
	
	//bind 到本地地址，端口为12345
	bind(listenfd,(struct sockaddr *)&servaddr,&servaddr);
	//listen的backlog为1024
	listen(listenfd,1024);
	//循环处理用户请求
	for(;;)
	{
		clilen = sizeof(cliaddr);
		connfd = accept(listenfd,(struct sockaddr *)&cliaddr,&clilen);
		read_data(connfd);//读取数据
		close(connfd);//关闭连接套接字，注意不是监听套接字
	}
}
*/
//29-43 行先后创建了 socket 套接字，bind 到对应地址和端口，并开始调用 listen 接口监听；
//46-50 行循环等待连接，通过 accept 获取实际的连接，并开始读取数据；
//16-23 行实际每次读取 1K 数据，之后休眠 1 秒，用来模拟服务器端处理时延。






