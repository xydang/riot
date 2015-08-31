#ifndef __GPIO_H__
#define __GPIO_H__

#define GPIO4_DR        0x20A8000
#define GPIO4_GDIR      0x20A8004
#define GPIO5_DR        0x20AC000
#define GPIO5_GDIR      0x20AC004

#define IOMUXC          0x20E0000
#define IOMUXC_GPIO4_16 0x20E009C
#define IOMUXC_GPIO4_17 0x20E00A0
#define IOMUXC_GPIO4_18 0x20E00A4
#define IOMUXC_GPIO4_19 0x20E00A8
#define IOMUXC_GPIO     0x5

#define GPIO4_16 112
#define GPIO4_17 113
#define GPIO4_18 114
#define GPIO4_19 115
#define GPIO4_26 122
#define GPIO4_27 123
#define GPIO4_31 127
#define GPIO5_5  133
#define GPIO5_6  134
#define GPIO5_7  135
#define GPIO5_8  136

void gpio_init();
void gpio_dir(int gpio,int dir);
void gpio_set(int gpio,int value);
int gpio_get(int gpio);

#endif

