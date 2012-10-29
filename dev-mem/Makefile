TOOLDIR=../tools/arm-bcm2708/arm-bcm2708-linux-gnueabi/bin
XCC=$(TOOLDIR)/arm-bcm2708-linux-gnueabi-gcc

all: tryit

export: tryit
	scp tryit pi@rpi:/home/pi

tryit: tryit.c
	$(XCC) -o tryit tryit.c

clean:
	rm -f tryit *.o *~
