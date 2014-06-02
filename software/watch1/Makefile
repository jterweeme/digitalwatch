BOARD = ask1ca
MACHINE := -msmallc -msys-crt0='../bsp//obj/HAL/src/crt0.o' -msys-lib=hal_bsp

%.o:
	nios2-elf-g++ -c -I../bsp -I../bsp/HAL/inc -I../bsp/drivers/inc $<

.PHONY: all

all: main.hex watch1.png

watch1.png: watch1.uxf
	umlet -action=convert -format=png -filename=$< -output=$@

main.hex: main.flash
	nios2-elf-objcopy -I srec -O ihex $< $@

main.flash: main.elf
	elf2flash --epcs --after=../../$(BOARD).flash --input=$< --output=$@

main.elf: main.o uart.o timer.o segment.o rtc.o buttons.o
	nios2-elf-g++ -o main.elf -T'../bsp//linker.x' $(MACHINE) -L ../bsp $^

main.o: main.cpp
uart.o: uart.cpp uart.h
timer.o: timer.cpp misc.h
segment.o: segment.cpp
rtc.o: rtc.cpp rtc.h
buttons.o: buttons.cpp misc.h

download: main.elf
	nios2-download -g $<

clean:
	rm -v *.o *.elf *.hex *.flash *.png *.bak

