#ifndef __GPIO_H__
#define __GPIO_H__

#define GPIO4_16 112
#define GPIO4_17 113
#define GPIO4_18 114
#define GPIO4_19 115

void gpio_init();
void gpio_dir(int gpio,int dir);
void gpio_set(int gpio,int value);
int gpio_get(int gpio);

#endif

