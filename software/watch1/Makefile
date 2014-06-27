BOARD = ask1ca
MACHINE := -msmallc -msys-crt0='../bsp//obj/HAL/src/crt0.o' -msys-lib=hal_bsp
DOWNLOAD = app_watch1.elf

%.o:
	nios2-elf-g++ -Wall -c -I../bsp -I../bsp/HAL/inc $<

%.elf:
	nios2-elf-g++ -o $@ -T'../bsp//linker.x' $(MACHINE) -L ../bsp $^

.PHONY: all

all: app_watch1.hex app_teller1.elf

watch1.png: watch1.uxf
	umlet -action=convert -format=png -filename=$< -output=$@

app_watch1.hex: app_watch1.flash
	nios2-elf-objcopy -I srec -O ihex $< $@

app_watch1.flash: app_watch1.elf
	elf2flash --epcs --after=../../$(BOARD).flash --input=$< --output=$@

app_teller1.elf: app_teller1.o misc.o
app_watch1.elf: app_watch1.o misc.o mystl.o

app_teller1.o: app_teller1.cpp misc.h
app_watch1.o: app_watch1.cpp misc.h
misc.o: misc.cpp misc.h
mystl.o: mystl.cpp mystl.h

download: $(DOWNLOAD)
	nios2-download -g $<

clean:
	rm -Rvf *.o *.elf *.hex *.flash *.png *.bak

