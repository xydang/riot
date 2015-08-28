INCLUDE=-I ./include/
LIBS= -lpthread -ljson-c
all:iot
iot:
	gcc -o iot  main.c serial.c tcp.c reply.c mpl3115.c $(INCLUDE) $(LIBS) -O2 -Wno-deprecated-declarations -Wno-unused-result
clean:
	rm -rfv iot

