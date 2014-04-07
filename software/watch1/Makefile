BOARD = ask1ca
MACHINE := -msmallc -msys-crt0='../bsp//obj/HAL/src/crt0.o' -msys-lib=hal_bsp

all: main.hex watch1.png

watch1.png: watch1.uxf
	umlet -action=convert -format=png -filename=$< -output=$@

main.hex: main.flash
	nios2-elf-objcopy -I srec -O ihex $< $@

main.flash: main.elf
	elf2flash --epcs --after=../../$(BOARD).flash --input=$< --output=$@

main.elf: main.o uart.o timer.o segment.o ds1302.o buttons.o
	nios2-elf-g++ -o main.elf -T'../bsp//linker.x' $(MACHINE) -L ../bsp $^

main.o: main.cpp main.h
	nios2-elf-g++ -c -I../bsp -I../bsp/HAL/inc -I../bsp/drivers/inc main.cpp

uart.o: uart.cpp uart.h
	nios2-elf-g++ -c -I../bsp -I../bsp/HAL/inc -I../bsp/drivers/inc uart.cpp

timer.o: timer.cpp timer.h
	nios2-elf-g++ -c -I../bsp -I../bsp/HAL/inc -I../bsp/drivers/inc timer.cpp

segment.o: segment.cpp
	nios2-elf-g++ -c -I../bsp -I../bsp/HAL/inc -I../bsp/drivers/inc segment.cpp

ds1302.o: ds1302.cpp ds1302.h
	nios2-elf-g++ -c -I../bsp -I../bsp/HAL/inc -I../bsp/drivers/inc ds1302.cpp

buttons.o: buttons.cpp buttons.h observer.h
	nios2-elf-g++ -c -I../bsp -I../bsp/HAL/inc -I../bsp/drivers/inc buttons.cpp

download: main.elf
	nios2-download -g $<

clean:
	rm -v *.o *.elf *.hex *.flash *.png

