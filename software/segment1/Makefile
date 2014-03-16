BOARD = ask1ca
DIRECTIVES := -DALT_USE_SMALL_DRIVERS -DSMALL_C_LIB
MACHINE := -msmallc -msys-crt0='../bsp//obj/HAL/src/crt0.o' -msys-lib=hal_bsp

all:
	nios2-elf-g++ -c $(DIRECTIVES) -I../bsp main.cpp
	nios2-elf-g++ -o main.elf -T'../bsp//linker.x' $(MACHINE) -L ../bsp main.o
	elf2flash --epcs --after=../../$(BOARD).flash --input=main.elf --output=main.flash
	nios2-elf-objcopy -I srec -O ihex main.flash main.hex

download:
	nios2-download -g main.elf


