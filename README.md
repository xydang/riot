#基于RiotBoard的智能家居DEMO

#0.简介
本DEMO实现了通过微信检测传感器数据，并且可以实时控制家里面的各种电器。

#1.云平台
云平台使用的是[乐为物联](http://www.lewei50.com/)，免费账号对传感器数量有限制，不过也够用了。通过云将RiotBoard接入云平台，可以方便的实现微信控制家里的电器。
#2.传感器
目前只加了一个传感器，飞思卡尔的[MPL3115A2]([http://www.freescale.com/products/sensors/pressure-sensors/barometric-pressure-15-to-115-kpa/20-to-110kpa-absolute-digital-pressure-sensor:MPL3115A2](http://www.freescale.com/products/sensors/pressure-sensors/barometric-pressure-15-to-115-kpa/20-to-110kpa-absolute-digital-pressure-sensor:MPL3115A2))高精度数字气压传感器，通过这个传感器可以获得温度、气压数据，通过此程序可以将其实时更新到云平台。
#3.反向控制
目前只实现了GPIO与UART的控制，不过把UART接口与ZigBee模块连接后，就可以扩展出很多新玩法了。至于GPIO就先接几个继电器吧。

#4.通信协议
本程序是通过TCP长连接方式更新传感器数据与反向控制。上下行的数据都是JSON格式，所以用到了一个json-c的库。[具体协议点这里]([http://www.lewei50.com/dev/doc/176)。
#5.关于RiotBoard
[RiotBoard](http://riotboard.org/)是英蓓特面向于高端移动设备、高端掌上电脑、高端便携式媒体播放器、游戏机、上网本、桌面一体机、便携式导航设备等领域推出的一款基于飞思卡尔（Freescale Semiconductor）i.MX 6Solo处理器的评估板，板载板载4GB4GByte eMMC、2*512MB DDR3 SDRAM， 4路串口，4路USB Host, USB OTG， LVDS，HDMI，音频输入输出，SD卡，TF卡，10M/100M/1G自适应网口，JTAG，Camera，MIPI等接口.


