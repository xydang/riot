INCLUDE=-I ./include/
LIBS= -lpthread -ljson-c
all:iot
iot:
	gcc -o iot  main.c serial.c tcp.c reply.c  $(INCLUDE) $(LIBS) -Wno-deprecated-declarations
clean:
	rm -rfv iot

