# 2014 Jasper ter Weeme

all:
	cd bsp && ./create-this-bsp
	make -C leds1
	make -C segment1
	make -C watch1

clean:
	rm -Rvf bsp/drivers bsp/HAL bsp/Makefile bsp/*.mk bsp/*.html
	make -C watch1 clean

