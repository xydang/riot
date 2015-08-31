#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "gpio.h"

#define MAP_SIZE 4096UL
#define MAP_MASK (MAP_SIZE - 1)

unsigned long reg_read(off_t addr)
{
	int memfd;
	void *map_base, *virt_addr;
	unsigned long val;

	if((memfd = open("/dev/mem", O_RDWR | O_SYNC)) == -1)
		exit(1);

	map_base = mmap(0,MAP_SIZE,PROT_READ | PROT_WRITE,MAP_SHARED,memfd,addr & ~MAP_MASK);
	if(map_base == (void *) -1)
                        exit(1);
	virt_addr = map_base + (addr & MAP_MASK);
	val=*((unsigned long *) virt_addr);
	if(munmap(map_base, MAP_SIZE) == -1)
		exit(1);
	close(memfd);
	return val;
}

void reg_write(unsigned long addr,unsigned long dat)
{
        int memfd;
        void *map_base, *virt_addr;

        if((memfd = open("/dev/mem", O_RDWR | O_SYNC)) == -1)
                        exit(1);

        map_base = mmap(0,MAP_SIZE,PROT_READ | PROT_WRITE,MAP_SHARED,memfd,addr & ~MAP_MASK);
        if(map_base == (void *) -1)
                        exit(1);
        virt_addr = map_base + (addr & MAP_MASK);
        *((unsigned long *) virt_addr) = dat;
        close(memfd);
}

void gpio_init()
{
	/* Configure IOMUX to select GPIO mode */
	reg_write(IOMUXC_GPIO4_16,IOMUXC_GPIO);
        reg_write(IOMUXC_GPIO4_17,IOMUXC_GPIO);
        reg_write(IOMUXC_GPIO4_18,IOMUXC_GPIO);
        reg_write(IOMUXC_GPIO4_19,IOMUXC_GPIO);
	reg_write(IOMUXC_GPIO4_26,IOMUXC_GPIO);
	reg_write(IOMUXC_GPIO4_27,IOMUXC_GPIO);
	reg_write(IOMUXC_GPIO4_31,IOMUXC_GPIO);
	reg_write(IOMUXC_GPIO5_05,IOMUXC_GPIO);
	reg_write(IOMUXC_GPIO5_06,IOMUXC_GPIO);
	reg_write(IOMUXC_GPIO5_07,IOMUXC_GPIO);
	reg_write(IOMUXC_GPIO5_08,IOMUXC_GPIO);
}

void gpio_dir(int gpio,int dir)
{
	unsigned long gdir;
	unsigned char port;
	
	gpio-=96;
	if(gpio>31){
		gpio-=32;
		port=5;
		gdir=reg_read(GPIO5_GDIR);
	}else{
		port=4;
		gdir=reg_read(GPIO4_GDIR);
	}
	
	if(dir){
		if(port==4)
			reg_write(GPIO4_GDIR,gdir|(1<<gpio));
		else
			reg_write(GPIO5_GDIR,gdir|(1<<gpio));
	}else{
		if(port==4)
			reg_write(GPIO4_GDIR,gdir&(~(1<<gpio)));
		else
			reg_write(GPIO5_GDIR,gdir&(~(1<<gpio)));
	}
}

void gpio_set(int gpio,int value)
{
	unsigned long dr;
	unsigned char port;
	
	gpio-=96;
	if(gpio>31){
		gpio-=32;
		port=5;
		dr=reg_read(GPIO5_DR);
	}else{
		port=4;
		dr=reg_read(GPIO4_DR);
	}
	
	if(value){
		if(port==4)
			reg_write(GPIO4_DR,dr|(1<<gpio));
		else
			reg_write(GPIO5_DR,dr|(1<<gpio));
	}else{
		if(port==4)
			reg_write(GPIO4_DR,dr&(~(1<<gpio)));
		else
			reg_write(GPIO5_DR,dr&(~(1<<gpio)));
	}
}		

int gpio_get(int gpio)
{
	unsigned long dr;
	
	gpio-=96;
	if(gpio>31){
		gpio-=32;
		dr=reg_read(GPIO5_DR);
	}else{
		dr=reg_read(GPIO4_DR);
	}
	if(dr&(1<<gpio))
		return 1;
	else
		return 0;
}

