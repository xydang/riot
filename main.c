#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <pthread.h>
#include "tcp.h"
#include "json-c/json.h"

#define SERVER "tcp.lewei50.com"
#define PORT   9960
#define HEART_BEAT "{\"method\":\"update\",\"gatewayNo\":\"01\",\"userkey\":\"acf941838c3045488a84399c3d9b08ee\"}&^!"
#define RESPONSE_DEV "{\"method\": \"response\",\"result\": {\"successful\":true,\"message\": \"ok\",\"data\":[{\"id\":\"sw0\",\"value\":\"0\"},{\"id\":\"sw1\",\"value\":\"0\"},{\"id\":\"sw2\",\"value\":\"0\"},{\"id\":\"sw3\",\"value\":\"0\"}]}}&^!"
#define RESPONSE_ACK "{\"method\":\"response\",\"result\":{\"successful\":true,\"message\":\"ok\"}}&^!"
#define SENSOR_DAT "{\"method\": \"upload\",\"data\":[{\"Name\":\"T1\",\"Value\":\"%2.1f\"},{\"Name\":\"H1\",\"Value\":\"%3d\"}]}&^!"


void msg_handle(char *msg_buf,int len)
{
	json_object *new_obj;

	new_obj = json_tokener_parse(msg_buf);
	json_object *obj= json_object_object_get(new_obj,"f");
	char *method=(char *)json_object_get_string(obj);
	if(strcmp(method,"getAllSensors")==0){
		send(sockfd,RESPONSE_DEV,strlen(RESPONSE_DEV),0);
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
		send(sockfd,RESPONSE_ACK,strlen(RESPONSE_ACK),0);
		json_object_put(dev_obj);
		json_object_put(val_obj);
	}else if(strcmp(method,"writeSerial")==0){
		json_object *val_obj= json_object_object_get(new_obj,"p1");
		send(sockfd,RESPONSE_ACK,strlen(RESPONSE_ACK),0);
		printf("write serial:%s\n",json_object_get_string(val_obj));
		json_object_put(val_obj);
	}else{
		printf("%s\n", json_object_to_json_string(new_obj));
		send(sockfd,RESPONSE_ACK,strlen(RESPONSE_ACK),0);
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
	int n=0,k=0;
	char send_buf[1024];

	while(1){
		n++;
		k++;
		if(n>30){
			n=0;
			send(sockfd,HEART_BEAT,strlen(HEART_BEAT),0);
		}
		if(k>10){
			k=0;
			sprintf(send_buf,SENSOR_DAT,(rand()%500)/10.0,rand()%1000);
			send(sockfd,send_buf,strlen(send_buf),0);
		}
		sleep(1);	
	}

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











