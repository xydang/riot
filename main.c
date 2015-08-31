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
#include "gpio.h"

char userid[36],devno[4];

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

void sys_init()
{
	gpio_init();
	gpio_dir(GPIO4_16,1);
	gpio_dir(GPIO4_17,1);
	gpio_dir(GPIO4_18,1);
	gpio_dir(GPIO4_19,1);
}

int main(int argc,char *argv[])
{
	FILE *ffd;
	char server[32];
	int port;
	pthread_t thread_recv,thread_send;
	
	ffd=fopen("./iot.conf","r");
	if(ffd<0){
		printf("open iot.conf error");
		exit(1);
	}
	fscanf(ffd,"userid=%s\ndevno=%s\nserver=%s\nport=%d\n",userid,devno,server,&port);	
	fclose(ffd);
	
	sys_init();

	sockfd=tcp_open(server,port);

	pthread_create(&thread_recv,NULL,task_recv,NULL);
	pthread_create(&thread_send,NULL,task_send,NULL);
	
	printf("\niot server started....\n");
	printf("server=%s\tport=%d\n\n",server,port);
	while(1);
	close(sockfd);
	
	return 0;
}






