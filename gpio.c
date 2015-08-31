#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

void gpio_init()
{
        system("devmem2 0x20E009C w 0x5");// enable GPIO4_16
        system("devmem2 0x20E00A0 w 0x5");// enable GPIO4_17
        system("devmem2 0x20E00A4 w 0x5");// enable GPIO4_18
        system("devmem2 0x20E00A8 w 0x5");// enable GPIO4_19

        system("echo 112 > /sys/class/gpio/export");
        system("echo 113 > /sys/class/gpio/export");
        system("echo 114 > /sys/class/gpio/export");
        system("echo 115 > /sys/class/gpio/export");
}

void gpio_dir(int gpio,int dir)
{
        char cmd_buf[48];

        if(dir)
                sprintf(cmd_buf,"echo out > /sys/class/gpio/gpio%d/direction",gpio);
        else
                sprintf(cmd_buf,"echo in > /sys/class/gpio/gpio%d/direction",gpio);
        system(cmd_buf);
}

void gpio_set(int gpio,int value)
{
        char cmd_buf[48];

        if(value)
                sprintf(cmd_buf,"echo 1 > /sys/class/gpio/gpio%d/value",gpio);
        else
                sprintf(cmd_buf,"echo 0 > /sys/class/gpio/gpio%d/value",gpio);
        system(cmd_buf);
}

int gpio_get(int gpio)
{
        int fd,len;
        char cmd_buf[48];
        char ch[2];

        memset(ch,0,2);
        sprintf(cmd_buf,"/sys/class/gpio/gpio%d/value",gpio);
        fd=open(cmd_buf,O_WRONLY);
        len=read(fd,ch,1);
        close(fd);

        return ch[0];
}

