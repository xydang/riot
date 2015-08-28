#ifndef __REPLAY_H__
#define __REPLAY_H__

void reply_heart_beat();
void replay_sta(int success,char *msg);
void replay_sensor_list();
extern char send_buf[1024];


#endif
