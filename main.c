#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "tcp.h"
#include "reply.h"
#include "mpl3115.h"
#include "msg.h"

#define SERVER "tcp.lewei50.com"
#define PORT   9960

void *task_recv(void *arg)
{
	int n=0;
	struct timespec ts = {0,10000};
	
	while(1){
		n=recv(sockfd,tcp_buf,1024,0);
		msg_handle(tcp_buf,n);
		memset(tcp_buf,0,n);
		n=0;
		nanosleep(&ts, NULL);
	}
}

void *task_send(void *arg)
{
	int n=29,k=9,i2cfd;
	char send_buf[1024];

	i2cfd=mpl3115_init(i2cfd);
	sleep(1);
	while(1){
		n++;
		k++;
		if(n>30){
			n=0;
			reply_heart_beat();
		}
		if(k>10){
			k=0;
			reply_update(i2cfd);
		}
		sleep(1);	
	}
	close(i2cfd);
}


int main(int argc,char *argv[])
{
	pthread_t thread_recv,thread_send;

	sockfd=tcp_open(SERVER,PORT);

	pthread_create(&thread_recv,NULL,task_recv,NULL);
	pthread_create(&thread_send,NULL,task_send,NULL);
	
	printf("task begin.\n");

	while('q'!=getchar());
	close(sockfd);

	return 0;
}






