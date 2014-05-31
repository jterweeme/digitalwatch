BOARD = ask1ca
MACHINE := -msmallc -msys-crt0='../bsp//obj/HAL/src/crt0.o' -msys-lib=hal_bsp


all: teller1.elf

teller1.hex: teller1.flash
	nios2-elf-objcopy -I srec -O ihex $< $@

teller1.flash: main.elf
	elf2flash --epcs --after=../../$(BOARD).flash --input=$< --output=$@

teller1.elf: teller1.o
	nios2-elf-g++ -o $@ -T'../bsp//linker.x' $(MACHINE) -L ../bsp $^

teller1.o: teller1.cpp
	nios2-elf-g++ -c -I../bsp -I../bsp/HAL/inc -I../bsp/drivers/inc $<

download:
	nios2-download -g teller1.elf

clean:
	rm -Rvf *.o *.elf

