#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <pthread.h>
#include "tcp.h"
#include "json-c/json.h"
#include "reply.h"
#include "mpl3115.h"

#define SERVER "tcp.lewei50.com"
#define PORT   9960

void msg_handle(char *msg_buf,int len)
{
	json_object *new_obj;

	new_obj = json_tokener_parse(msg_buf);
	json_object *obj= json_object_object_get(new_obj,"f");
	char *method=(char *)json_object_get_string(obj);
	if(strcmp(method,"getAllSensors")==0){
		replay_sensor_list();
	}else if(strcmp(method,"updateSensor")==0){
		json_object *dev_obj= json_object_object_get(new_obj,"p1");
		json_object *val_obj= json_object_object_get(new_obj,"p2");
		if(strcmp(json_object_get_string(dev_obj),"sw0")==0){
			if(strcmp(json_object_get_string(val_obj),"1")==0){
				system("echo 1 > /sys/class/leds/user_led/brightness");
				printf("sw0 on.\n");	
			}else{
				system("echo 0 > /sys/class/leds/user_led/brightness");
				printf("sw0 off.\n");
			}
		}			
		replay_sta(1,"ok");
		json_object_put(dev_obj);
		json_object_put(val_obj);
	}else if(strcmp(method,"writeSerial")==0){
		json_object *val_obj= json_object_object_get(new_obj,"p1");
		replay_sta(1,"ok");
		printf("write serial:%s\n",json_object_get_string(val_obj));
		json_object_put(val_obj);
	}else{
		printf("%s\n", json_object_to_json_string(new_obj));
		replay_sta(1,"ok");
	}
	
	json_object_put(new_obj);
	json_object_put(obj);
}

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
	int n=0,k=0,i2cfd;
	char send_buf[1024];

	if ((i2cfd = open("/dev/i2c-3", O_RDWR)) < 0){
		perror("error openning i2c-3!\n");
		exit(1);
	}
	mpl3115_init(i2cfd);
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






